#include "rnpch.h"
#include "State.h"
#include "StateStack.h"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player1, Player& player2)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, player1(&player1)
	, player2(&player2)
{
}

State::State(StateStack& stack, Context context)
	: stack_(&stack)
	, context_(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	stack_->pushState(stateID);
}

void State::requestStackPop()
{
	stack_->popState();
}

void State::requestStateClear()
{
	stack_->clearStates();
}

State::Context State::getContext() const
{
	return context_;
}