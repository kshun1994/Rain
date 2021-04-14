#include <Application.hpp>
#include <Utility.hpp>
#include <State.hpp>
#include <StateIdentifiers.hpp>
#include <TitleState.hpp>
#include <GameState.hpp>
#include <MenuState.hpp>
#include <PauseState.hpp>


const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(1280, 720), "Window", sf::Style::Titlebar | sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayer()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
	, mStatsText()
	, mStatsUpdateTime()
	, mStatsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::ID::Main, "Media/Font/CC-Astro-City.ttf");
	mTextures.load(Textures::ID::TitleScreen, "Media/Texture/State/Title.png");
	mTextures.load(Textures::ID::MainMenu, "Media/Texture/State/MainMenu.png");

	mStatsText.setFont(mFonts.get(Fonts::ID::Main));
	mStatsText.setPosition(5.f, 5.f);
	mStatsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::ID::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatsUpdateTime += dt;
	mStatsNumFrames += 1;
	if (mStatsUpdateTime >= sf::seconds(1.0f))
	{
		mStatsText.setString("Updates per second: " + std::to_string(mStatsNumFrames));

		mStatsUpdateTime -= sf::seconds(1.0f);
		mStatsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::ID::Title);
	mStateStack.registerState<MenuState >(States::ID::Menu);
	mStateStack.registerState<GameState >(States::ID::Game);
	mStateStack.registerState<PauseState>(States::ID::Pause);
}
