#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Player.h"
#include "StateStack.h"
#include "InputDevice.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
public:
								Application();
	void						run();


private:
	void						processInput();
	void						update(Player::TaggedInput player1Input, Player::TaggedInput player2Input);
	void						render();

	void						updateStatistics(float ft, int numUpdates);
	void						registerStates();


private:
	static const sf::Time		TimePerFrame;

	sf::RenderWindow			mWindow;
	TextureHolder				mTextures;
	FontHolder					mFonts;

	Player						mPlayer1;
	Player						mPlayer2;
	Player::TaggedInput			mPlayer1Input;
	Player::TaggedInput			mPlayer2Input;

	StateStack					mStateStack;

	float						mCurrentSlice;
	float						mLastFT;

	float						mStatsTimer;
	int							mTotalUpdates;
	int							mUpdatesPerSec;
	sf::Text					mStatsText;
};