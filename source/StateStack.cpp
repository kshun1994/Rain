#include "rnpch.h"
#include "StateStack.h"


StateStack::StateStack(State::Context context)
	: stack_()
	, pendingList_()
	, context_(context)
	, factories_()
{
}

void StateStack::update()
{
	// Iterate from top to bottom, stop as soon as update() returns false
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
	{
		if (!(*itr)->update())
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	// Draw all active states from bottom to top
	for (State::Ptr & state : stack_)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = stack_.rbegin(); itr != stack_.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
	pendingList_.push_back(PendingChange(Push, stateID));
	RN_DEBUG("State push requested - {}.", magic_enum::enum_name(stateID));
	RN_DEBUG("Current stack size - {}.", stack_.size());
}

void StateStack::popState()
{
	pendingList_.push_back(PendingChange(Pop));
	RN_DEBUG("State pop requested.");
	RN_DEBUG("Current stack size - {}.", stack_.size());
}

void StateStack::clearStates()
{
	pendingList_.push_back(PendingChange(Clear));
	RN_DEBUG("Stack clear requested.");
	RN_DEBUG("Current stack size - {}.", stack_.size());
}

int StateStack::getStackSize() const
{
	return stack_.size();
}

bool StateStack::isEmpty() const
{
	return stack_.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = factories_.find(stateID);
	assert(found != factories_.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : pendingList_)
	{
		switch (change.action)
		{
		case Action::Push:
			stack_.push_back(createState(change.stateID));
			RN_DEBUG("State pushed - {}.", magic_enum::enum_name(change.stateID));
			break;

		case Action::Pop:
			stack_.pop_back();
			RN_DEBUG("State popped.");
			break;

		case Action::Clear:
			stack_.clear();
			RN_DEBUG("Stack cleared.");
			break;
		}
		RN_DEBUG("Current stack size - {}", stack_.size());
	}

	pendingList_.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}
