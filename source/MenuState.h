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
	virtual bool			update(unsigned int player1Input, unsigned int player2Input);
	virtual bool			handleEvent(const sf::Event& event);

private:
	sf::Sprite				mBackgroundSprite;
	GUI::Container			mGUIContainer;

};