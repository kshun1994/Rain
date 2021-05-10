#include "rnpch.h"
#include "GameState.h"


GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window)
	, mPlayer1(*context.player1)
	, mPlayer2(*context.player2)
{
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update()
{
	// Read in accumulated player input for current update and add to input buffer

	// Check hitbox/hurtbox overlaps

	// Check if player characters are actionable

		// If actionable, initiate action based on input buffer readout


	mWorld.update();

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