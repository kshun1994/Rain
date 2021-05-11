#pragma once

#include "StateStack.h"
#include "World.h"
#include "Player.h"


class GameState : public State
{
public:
								GameState(StateStack& stack, Context context);
	virtual void				draw();
	virtual bool				update(unsigned int player1Input, unsigned int player2Input);
	virtual bool				handleEvent(const sf::Event& event);

private:
	void						updateInputBuffer(int player1Input, int player2Input);

private:
	World						mWorld;

	Player&						mPlayer1;
	Player&						mPlayer2;
};