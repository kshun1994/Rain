#pragma once
#include "CharState.h"
#include "StateStack.h"

// An Action is a set of states that comprise a character action
// For example, one Action may be three states: a startup state, an active state, and a recovery state
// A Character should own a vector of heap-allocated pointers to all performable Actions
// Every update, a Character object checks Player input against inputs required to call Actions
// Once an Action is called, it should put its component states on the relevant character's state stack

class Action
{
public:
										Action() = default;
										Action(StateStack& stateStack);

	void								addState(CharState& state);

private:
	void								pushAction();

private:
	int&								playerInput_;
	//std::vector<CharState&>				states_;
};
