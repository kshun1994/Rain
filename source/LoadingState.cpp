#include "rnpch.h"
#include "LoadingState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


LoadingState::LoadingState(StateStack& stack, Context context)
: State(stack, context)
{
	sf::RenderWindow& window = *getContext().window;
	sf::Font& font = context.fonts->get(Fonts::ID::Main);
	sf::Vector2f viewSize = window.getView().getSize();

	loadingText_.setFont(font);
	loadingText_.setString("Loading Resources");
	centerOrigin(loadingText_);
	loadingText_.setPosition(viewSize.x / 2.f, viewSize.y / 2.f + 50.f);

	progressBarBackground_.setFillColor(sf::Color::White);
	progressBarBackground_.setSize(sf::Vector2f(viewSize.x - 20, 10));
	progressBarBackground_.setPosition(10, loadingText_.getPosition().y + 40);

	progressBar_.setFillColor(sf::Color(100,100,100));
	progressBar_.setSize(sf::Vector2f(200, 10));
	progressBar_.setPosition(10, loadingText_.getPosition().y + 40);

	setCompletion(0.f);

	loadingTask_.execute();
}

void LoadingState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(loadingText_);
	window.draw(progressBarBackground_);
	window.draw(progressBar_);
}

bool LoadingState::update()
{
	// Update the progress bar from the remote task or finish it
	if (loadingTask_.isFinished())
	{
		requestStackPop();
		requestStackPush(States::ID::Game);
	}
	else
	{
		setCompletion(loadingTask_.getCompletion());
	}
	return true;
}

bool LoadingState::handleEvent(const sf::Event& event)
{
	return true;
}

void LoadingState::setCompletion(float percent)
{
	if (percent > 1.f) // clamp
		percent = 1.f;

	progressBar_.setSize(sf::Vector2f(progressBarBackground_.getSize().x * percent, progressBar_.getSize().y));
}
