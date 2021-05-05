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
	void						update();
	void						render();

	void						updateStatistics(float ft);
	void						registerStates();


private:
	static const sf::Time		TimePerFrame;

	sf::RenderWindow			mWindow;
	TextureHolder				mTextures;
	FontHolder					mFonts;
	Player						mPlayer;

	StateStack					mStateStack;

	float						mCurrentSlice;
	float						mLastFT;

	sf::Text					mStatsText;
};