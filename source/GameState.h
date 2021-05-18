#pragma once

#include "StateStack.h"
#include "World.h"
#include "Player.h"


class GameState : public State
{
public:
								GameState(StateStack& stack, Context context);
								~GameState();

	virtual void				draw();
	virtual bool				update();
	virtual bool				handleEvent(const sf::Event& event);

private:
	World						mWorld;

	Player&						mPlayer1;
	Player&						mPlayer2;
};