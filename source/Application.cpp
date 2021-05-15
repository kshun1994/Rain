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


Application::Application()
	: mWindow(sf::VideoMode(1280, 720), "Window", sf::Style::Titlebar | sf::Style::Close)
	, mTextures()
	, mFonts()
	, mPlayer1(Player::ID::Player1)
	, mPlayer2(Player::ID::Player2)
	, mPlayer1Input(Player::ID::Player1, 0)
	, mPlayer2Input(Player::ID::Player2, 0)
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer1, mPlayer2))
	, mCurrentSlice(0.f)
	, mLastFT(0.f)
	, mStatsText()
	, mTotalUpdates(0)
	, mUpdatesPerSec(0)
	, mStatsTimer(0)
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setFramerateLimit(0);

	// // Register connected joysticks (SFML supports up to 8)
	// for (int ID = 0; ID < 7; ID++)
	// {
	// 	if (sf::Joystick::isConnected(ID))
	// 	{
	// 		// Assign joystick to InputDevice instance if connected
	// 	}
	// }

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


	update(mPlayer1Input, mPlayer2Input);

	for (int i = 0; i < 8; i++)
	{
		if (sf::Joystick::isConnected(i))
		{
			RN_DEBUG("Joystick ID {} is connected.", i);
		}
	}
}

void Application::run()
{
	while (mWindow.isOpen())
	{
		auto timePoint1(std::chrono::high_resolution_clock::now());

		//for (int i = 0; i < 8; i++)
		//{
		//	for (int button = 0; button < sf::Joystick::getButtonCount(i); button++)
		//	{
		//		if (sf::Joystick::isButtonPressed(i, button))
		//		{
		//			RN_DEBUG("Joystick {0} -- Button {1} is pressed.", i, button);
		//		}

		//		if (sf::Joystick::hasAxis(i, sf::Joystick::Z))
		//		{
		//			RN_DEBUG("Joystick {0} -- Z Axis position {1}.", i, sf::Joystick::getAxisPosition(i, sf::Joystick::Z));
		//		}

		//		if (sf::Joystick::hasAxis(i, sf::Joystick::R))
		//		{
		//			RN_DEBUG("Joystick {0} -- R Axis position {1}.", i, sf::Joystick::getAxisPosition(i, sf::Joystick::R));
		//		}

		//		if (sf::Joystick::hasAxis(i, sf::Joystick::U))
		//		{
		//			RN_DEBUG("Joystick {0} -- U Axis position {1}.", i, sf::Joystick::getAxisPosition(i, sf::Joystick::U));
		//		}

		//		if (sf::Joystick::hasAxis(i, sf::Joystick::V))
		//		{
		//			RN_DEBUG("Joystick {0} -- V Axis position {1}.", i, sf::Joystick::getAxisPosition(i, sf::Joystick::V));
		//		}
		//	}
		//}

		processInput();

		mCurrentSlice += mLastFT;
		int mNumUpdates = 0;

		for (; mCurrentSlice >= CONST_TICK_DURATION; mCurrentSlice -= CONST_TICK_DURATION)
		{
			mPlayer1Input = mPlayer1.getAccumulatedInput();
			mPlayer2Input = mPlayer2.getAccumulatedInput();

			update(mPlayer1Input, mPlayer2Input);
			mNumUpdates++;

			// Clear latest update's input from next set of accumulations
			mPlayer1.clearAccumulatedInput();
			mPlayer2.clearAccumulatedInput();
		}

		if (mStateStack.isEmpty())
			mWindow.close();

		render();

		auto timePoint2(std::chrono::high_resolution_clock::now());
		auto elapsedTime(timePoint2 - timePoint1);
		mLastFT = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(elapsedTime).count();

		updateStatistics(mLastFT, mNumUpdates);
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

	// Get current input state every slice and then accumulate into a single input
	mPlayer1.accumulateInput(mPlayer1.getCurrentInputState());
	mPlayer2.accumulateInput(mPlayer2.getCurrentInputState());
}

void Application::update(Player::TaggedInput mPlayer1Input, Player::TaggedInput mPlayer2Input)
{
	mStateStack.update(mPlayer1Input, mPlayer2Input);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());

	#ifdef RN_DEBUG
		mWindow.draw(mStatsText);
	#endif // RN_DEBUG

	mWindow.display();
}

void Application::updateStatistics(float ft, int numUpdates)
{
	auto ftSeconds = ft / 1000.f;
	auto fps = 1.f / ftSeconds;

	mTotalUpdates += numUpdates;
	mStatsTimer += ft;
	while (mStatsTimer > 1000.f)
	{
		mUpdatesPerSec = mTotalUpdates;
		mTotalUpdates = 0;
		mStatsTimer -= 1000.f;
	}

	mStatsText.setString("Frame duration: " + std::to_string(ft) 
				        + "\nFrames/sec.: " + std::to_string(fps) 
					   + "\nUpdates/sec.: " + std::to_string(mUpdatesPerSec));
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState   >(States::ID::Title);
	mStateStack.registerState<MenuState    >(States::ID::Menu);
	mStateStack.registerState<GameState    >(States::ID::Game);
	mStateStack.registerState<PauseState   >(States::ID::Pause);
	mStateStack.registerState<SettingsState>(States::ID::Settings);
}
