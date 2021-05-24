#pragma once

#include "StateStack.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class TitleState : public State
{
public:
								TitleState(StateStack& stack, Context context);
	virtual void				draw();
	virtual bool				update();
	virtual bool				handleEvent(const sf::Event& event);

private:
	sf::Sprite					backgroundSprite_;
	sf::Text					text_;

	bool						showText_;
	sf::Time					textEffectTime_;
};