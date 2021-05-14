#pragma once

#include "State.h"
#include "Container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class MenuState : public State
{
public:
							MenuState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(Player::TaggedInput player1Input, Player::TaggedInput player2Input);
	virtual bool			handleEvent(const sf::Event& event);

private:
	sf::Sprite				mBackgroundSprite;
	GUI::Container			mGUIContainer;

};