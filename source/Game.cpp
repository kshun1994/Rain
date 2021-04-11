#include <iostream>
#include <sstream>
#include <Game.hpp>

const float Game::PlayerSpeed = 200.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
: mWindow(sf::VideoMode(1280, 720), "Window", sf::Style::Titlebar | sf::Style::Close)
, mTexture()
, mPlayer()
, mFont()
, mStatsText()
, mStatsUpdateTime()
, mStatsNumFrames(0)
, mIsMovingUp(false)
, mIsMovingDown(false)
, mIsMovingRight(false)
, mIsMovingLeft(false)
{
	if (!mTexture.loadFromFile("sprites/yuzuriha/Yuz_000.png"))
	{
		std::cout << "Texture not loaded!\n";
	}
	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(500.f, 500.f);

	mFont.loadFromFile("fonts/CC-Astro-City.ttf");
	mStatsText.setFont(mFont);
	mStatsText.setPosition(5.f, 5.f);
	mStatsText.setCharacterSize(20);
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
			processEvents();
			update(TimePerFrame);
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

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				handlePlayerInput(event.key.code, true);
				break;

			case sf::Event::KeyReleased:
				handlePlayerInput(event.key.code, false);
				break;

			case sf::Event::Closed:
				mWindow.close();
				break;
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool bPressed)
{
	if		(key == sf::Keyboard::W)
		mIsMovingUp		= bPressed;
	else if (key == sf::Keyboard::S)
		mIsMovingDown	= bPressed;
	else if (key == sf::Keyboard::A)
		mIsMovingLeft	= bPressed;
	else if (key == sf::Keyboard::D)
		mIsMovingRight	= bPressed;
}

void Game::update(sf::Time dt)
{
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
		movement.x += PlayerSpeed;

	mPlayer.move(movement * dt.asSeconds());
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
	mWindow.draw(mPlayer);
	mWindow.draw(mStatsText);
	mWindow.display();
}


//class Game
//{
//
//public:
//	Game();
//	void run();
//
//private:
//	void processEvents(); // process player input
//	void update();
//	void render();
//
//private:
//	sf::RenderWindow mWindow;
//	sf::CircleShape mPlayer;
//
//};