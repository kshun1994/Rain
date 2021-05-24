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
	: window_(sf::VideoMode(constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT), "Window", sf::Style::Titlebar | sf::Style::Close)
	, textures_()
	, fonts_()
	, player1_(Player::ID::Player1)
	, player2_(Player::ID::Player2)
	, stateStack_(State::Context(window_, textures_, fonts_, player1_, player2_))
	, currentSlice_(0.f)
	, lastFT_(0.f)
	, statsText_()
	, totalUpdates_(0)
	, updatesPerSec_(0)
	, statsTimer_(0)
{
	// Window settings
	window_.setKeyRepeatEnabled(false);
	window_.setFramerateLimit(0);

	// Load fonts
	fonts_.load(Fonts::ID::Main, "media/font/CarroisGothicSC-Regular.ttf");
	RN_DEBUG("Font(s) loaded.");

	// Load textures
	textures_.load(Textures::ID::TitleScreen,    "media/texture/state/Title.png");
	textures_.load(Textures::ID::MainMenu,		 "media/texture/state/MainMenu.png");
	textures_.load(Textures::ID::ButtonNormal,	 "media/texture/ui/ButtonNormal.png");
	textures_.load(Textures::ID::ButtonSelected, "media/texture/ui/ButtonSelected.png");
	textures_.load(Textures::ID::ButtonPressed,	 "media/texture/ui/ButtonPressed.png");
	RN_DEBUG("Textures(s) loaded.");

	// Set FPS/update stats stuff
	statsText_.setFont(fonts_.get(Fonts::ID::Main));
	statsText_.setPosition(5.f, 5.f);
	statsText_.setCharacterSize(20u);

	registerStates();
	RN_DEBUG("States registered.");
	stateStack_.pushState(States::ID::Title);

	// Run update() once so TitleState actually gets pushed to StateStack
	update();

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
	while (window_.isOpen())
	{
		auto timePoint1(std::chrono::high_resolution_clock::now());

		// Accumulate player inputs over time
		processInput();

		currentSlice_ += lastFT_;
		int numUpdates_ = 0;

		for (; currentSlice_ >= constants::TICK_DURATION; currentSlice_ -= constants::TICK_DURATION)
		{
			update();
			numUpdates_++;

			// Clear latest update's input from next set of accumulations
			player1_.clearAccumulatedInput();
			player2_.clearAccumulatedInput();
		}

		if (stateStack_.isEmpty())
			window_.close();

		render();

		auto timePoint2(std::chrono::high_resolution_clock::now());
		auto elapsedTime(timePoint2 - timePoint1);
		lastFT_ = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(elapsedTime).count();

		updateStatistics(lastFT_, numUpdates_);
	}
}

void Application::processInput()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		stateStack_.handleEvent(event);

		if (event.type == sf::Event::Closed)
			window_.close();
	}

	// Get current input state every slice and then accumulate into a single input
	player1_.accumulateInput(player1_.getCurrentInputState());
	player2_.accumulateInput(player2_.getCurrentInputState());
}

void Application::update()
{
	stateStack_.update();
}

void Application::render()
{
	window_.clear();

	stateStack_.draw();

	window_.setView(window_.getDefaultView());

	#ifdef RN_DEBUG
		window_.draw(statsText_);
	#endif // RN_DEBUG

	window_.display();
}

void Application::updateStatistics(float ft, int numUpdates)
{
	auto ftSeconds = ft / 1000.f;
	auto fps = 1.f / ftSeconds;

	totalUpdates_ += numUpdates;
	statsTimer_ += ft;
	while (statsTimer_ > 1000.f)
	{
		updatesPerSec_ = totalUpdates_;
		totalUpdates_ = 0;
		statsTimer_ -= 1000.f;
	}

	statsText_.setString("Frame duration: " + std::to_string(ft) 
				        + "\nFrames/sec.: " + std::to_string(fps) 
					   + "\nUpdates/sec.: " + std::to_string(updatesPerSec_));
}

void Application::registerStates()
{
	stateStack_.registerState<TitleState   >(States::ID::Title);
	stateStack_.registerState<MenuState    >(States::ID::Menu);
	stateStack_.registerState<GameState    >(States::ID::Game);
	stateStack_.registerState<PauseState   >(States::ID::Pause);
	stateStack_.registerState<SettingsState>(States::ID::Settings);
}
