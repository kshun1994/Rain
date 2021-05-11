#include "rnpch.h"
#include "TitleState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mText()
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::TitleScreen));
	centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(context.window->getView().getSize() / 2.f);

	mText.setFont(context.fonts->get(Fonts::ID::Main));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	mText.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y - 50);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);

	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(unsigned int player1Input, unsigned int player2Input)
{
	// mTextEffectTime += dt;

	//if (mTextEffectTime >= sf::seconds(0.5f))
	//{
	//	mShowText = !mShowText; // toggle text visibility every half second
	//	mTextEffectTime = sf::Time::Zero;
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