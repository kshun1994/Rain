#pragma once

#include "StateStack.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitleState : public State
{
public:
								TitleState(StateStack& stack, Context context);
	virtual void				draw();
	virtual bool				update(Player::TaggedInput player1Input, Player::TaggedInput player2Input);
	virtual bool				handleEvent(const sf::Event& event);

private:
	sf::Sprite					mBackgroundSprite;
	sf::Text					mText;

	bool						mShowText;
	sf::Time					mTextEffectTime;
};