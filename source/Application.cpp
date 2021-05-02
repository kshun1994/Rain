#include "rnpch.h"

#include "Application.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SettingsState.h"


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

	mFonts.load(Fonts::ID::Main, "media/font/CarroisGothicSC-Regular.ttf");
	RN_DEBUG("Font(s) loaded.");

	mTextures.load(Textures::ID::TitleScreen,    "media/texture/state/Title.png");
	mTextures.load(Textures::ID::MainMenu,		 "media/texture/state/MainMenu.png");
	mTextures.load(Textures::ID::ButtonNormal,	 "media/texture/ui/ButtonNormal.png");
	mTextures.load(Textures::ID::ButtonSelected, "media/texture/ui/ButtonSelected.png");
	mTextures.load(Textures::ID::ButtonPressed,	 "media/texture/ui/ButtonPressed.png");
	RN_DEBUG("Textures(s) loaded.");

	mStatsText.setFont(mFonts.get(Fonts::ID::Main));
	mStatsText.setPosition(5.f, 5.f);
	mStatsText.setCharacterSize(20u);

	registerStates();
	RN_DEBUG("States registered.");
	mStateStack.pushState(States::ID::Title);

	RN_DEBUG("Current stack size - {}", mStateStack.getStackSize());
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
	RN_DEBUG("Previous stack size - {}", mStateStack.getStackSize());
	mStateStack.update(dt);
	RN_DEBUG("Current stack size -- {}", mStateStack.getStackSize());
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
	mStateStack.registerState<TitleState   >(States::ID::Title);
	mStateStack.registerState<MenuState    >(States::ID::Menu);
	mStateStack.registerState<GameState    >(States::ID::Game);
	mStateStack.registerState<PauseState   >(States::ID::Pause);
	mStateStack.registerState<SettingsState>(States::ID::Settings);
}
