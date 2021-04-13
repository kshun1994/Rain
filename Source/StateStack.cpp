#pragma once

#include <StateStack.hpp>

#include <cassert>

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, mContext));
	}
}

State::Ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID); // get smart pointer to object in wanted state class
	assert(found != mFactories.end()); // make sure wanted state class is in state map

	return found->second(); // return stored pointer to State object
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) // iterate active stack from end to beginning; from highest to lowest
	{
		if (!(*itr)->handleEvent(event)) // run each state's handleEvent() method and if returns false end loop
			return;
	}
	applyPendingChanges();
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
		state->draw(); // states can be transparent and will overlay each other
}

void StateStack::pushState(States::ID stateID) // add a state to active stack
{
	/* 
	The push/pop/clear functions can't alter the active stack immediately because 
	adding/removing objects during iteration would cause problems.
	Instead the alterations are appended to a list of pending operations which are 
	then carried out when applyPendingChanges() is called.
	*/

	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState() // remove state from active stack
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates() // completely empty active stack
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList) // iterate through pending changes list
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;

		case Pop:
			mStack.pop_back();
			break;
		
		case Clear:
			mStack.clear();
			break;
		}
	}

	mPendingList.clear();
}