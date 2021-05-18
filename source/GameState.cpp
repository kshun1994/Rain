#include "rnpch.h"
#include "GameState.h"


GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window)
	, mPlayer1(*context.player1)
	, mPlayer2(*context.player2)
{
}

GameState::~GameState()
{
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update()
{
	Player::TaggedInput P1Input = mPlayer1.getInput();
	Player::TaggedInput P2Input = mPlayer2.getInput();

	mWorld.update(P1Input, P2Input);

	CommandQueue& commands = mWorld.getCommandQueue();
	// mPlayer.handleRealtimeInput(commands);
	
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	// mPlayer.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::ID::Pause);

	return true;
}