#include "rnpch.h"
#include "TitleState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, text_()
	, showText_(true)
	, textEffectTime_(sf::Time::Zero)
{
	backgroundSprite_.setTexture(context.textures->get(Textures::ID::TitleScreen));
	centerOrigin(backgroundSprite_);
	backgroundSprite_.setPosition(context.window->getView().getSize() / 2.f);

	text_.setFont(context.fonts->get(Fonts::ID::Main));
	text_.setString("Press any key to start");
	centerOrigin(text_);
	text_.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y - 50);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(backgroundSprite_);

	if (showText_)
		window.draw(text_);
}

bool TitleState::update()
{
	// textEffectTime_ += dt;

	//if (textEffectTime_ >= sf::seconds(0.5f))
	//{
	//	showText_ = !showText_; // toggle text visibility every half second
	//	textEffectTime_ = sf::Time::Zero;
	//}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(States::ID::Menu);
	}
	
	return true;
}