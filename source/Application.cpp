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


const float ftSlice = 1000.f / 60.f;

Application::Application()
	: mWindow(sf::VideoMode(1280, 720), "Window", sf::Style::Titlebar | sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayer()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
	, mCurrentSlice(0.f)
	, mLastFT(0.f)
	, mStatsText()
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setFramerateLimit(60);

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

	update();
}

void Application::run()
{
	while (mWindow.isOpen())
	{
		auto timePoint1(std::chrono::high_resolution_clock::now());

		processInput();

		mCurrentSlice += mLastFT;

		for (; mCurrentSlice >= ftSlice; mCurrentSlice -= ftSlice)
		{
		update();
		}

		if (mStateStack.isEmpty())
			mWindow.close();

		render();

		auto timePoint2(std::chrono::high_resolution_clock::now());
		auto elapsedTime(timePoint2 - timePoint1);
		mLastFT = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(elapsedTime).count();

		updateStatistics(mLastFT);
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

void Application::update()
{
	mStateStack.update();
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatsText);

	mWindow.display();
}

void Application::updateStatistics(float ft)
{
	auto ftSeconds = ft / 1000.f;
	auto fps = 1.f / ftSeconds;

	mStatsText.setString("Frame duration: " + std::to_string(ft) + "\nFPS: " + std::to_string(fps));
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState   >(States::ID::Title);
	mStateStack.registerState<MenuState    >(States::ID::Menu);
	mStateStack.registerState<GameState    >(States::ID::Game);
	mStateStack.registerState<PauseState   >(States::ID::Pause);
	mStateStack.registerState<SettingsState>(States::ID::Settings);
}
