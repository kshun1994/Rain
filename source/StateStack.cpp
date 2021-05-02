#include "rnpch.h"
#include "StateStack.h"


StateStack::StateStack(State::Context context)
	: mStack()
	, mPendingList()
	, mContext(context)
	, mFactories()
{
}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	// Draw all active states from bottom to top
	for (State::Ptr & state : mStack)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
	RN_DEBUG("State pushed - {}.", stateID);
	RN_DEBUG("Current stack size - {}.", mStack.size());
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
	RN_DEBUG("State popped.");
	RN_DEBUG("Current stack size - {}.", mStack.size());
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
	RN_DEBUG("State stack cleared.");
	RN_DEBUG("Current stack size - {}.", mStack.size());
}

int StateStack::getStackSize() const
{
	return mStack.size();
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Action::Push:
			mStack.push_back(createState(change.stateID));
			break;

		case Action::Pop:
			mStack.pop_back();
			break;

		case Action::Clear:
			mStack.clear();
			break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}
