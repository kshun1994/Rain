#include "rnpch.h"
#include "GameState.h"


GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window)
	, mPlayer(*context.player)
{
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update()
{
	mWorld.update();

	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);
	
	RN_DEBUG("Current queue size - {}.", commands.getQueueSize());

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::ID::Pause);

	return true;
}