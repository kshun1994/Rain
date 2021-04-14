#include <iostream>
#include <sstream>
#include <Game.hpp>

const float Game::PlayerSpeed = 500.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const bool Game::bPauseOnLostFocus = true; // if true, game does not update when window is not in focus

//const int YuzuSpriteWidth = 864;
//const int YuzuSpriteHeight = 640;
//const int YuzuIdleFrameEnd = 36;
//const int YuzuFWalkFrameEnd = 48;
//const int YuzuBWalkFrameEnd = 60;

Game::Game()
: mWindow(sf::VideoMode(1280, 720), "Window", sf::Style::Titlebar | sf::Style::Close)
, mFont()
, mStatsText()
, mStatsUpdateTime()
, mStatsNumFrames(0)
, mWorld(mWindow)
{
	mFont.loadFromFile("media/font/CC-Astro-City.ttf");
	mStatsText.setFont(mFont);
	mStatsText.setPosition(5.f, 5.f);
	mStatsText.setCharacterSize(20);

	mIsPaused = false;
}

void Game::run()
{
	sf::Clock clock; // start clock
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen()) // main loop
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processInput();
			if (!mIsPaused)
			{
				update(TimePerFrame);
			}
			mStatsNumFrames += 1; // for calculating fps
		}
		// updateStatistics(elapsedTime);

		mStatsUpdateTime += dt; // cumulatively count elapsed time
		if (mStatsUpdateTime >= sf::seconds(1.0f)) // when elapsed time adds up to more than a second
		{
			mStatsText.setString("FPS: " + std::to_string(mStatsNumFrames));

			mStatsUpdateTime -= sf::seconds(1.0f);
			mStatsNumFrames = 0;
		}
		render();
	}
}

void Game::processInput()
{
	CommandQueue& commands = mWorld.getCommandQueue();

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::GainedFocus && bPauseOnLostFocus)
		{
			mIsPaused = false;
		}
		else if (event.type == sf::Event::LostFocus && bPauseOnLostFocus)
		{
			mIsPaused = true;
		}

		mPlayer.handleEvent(event, commands);

		if (event.type == sf::Event::Closed)
		{
			mWindow.close();
		}
	}

	mPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time dt)
{
	mWorld.update(dt);
}

void Game::updateStatistics(sf::Time dt)
{
	mStatsUpdateTime += dt;
	mStatsNumFrames += 1;

	if (mStatsUpdateTime >= sf::seconds(1.0f))
	{
		mStatsText.setString(
			"Frames / Second = " + std::to_string(mStatsNumFrames) + "\n" +
			"Time / Update = " + std::to_string(mStatsUpdateTime.asMicroseconds() / mStatsNumFrames) + "us");

		mStatsUpdateTime -= sf::seconds(1.0f);
		mStatsNumFrames = 0;
	}
}

void Game::render()
{
	mWindow.clear();
	mWorld.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatsText);
	mWindow.display();
}