#pragma once

#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PauseState : public State
{
	public:
							PauseState(StateStack& stack, Context context);

		virtual void		draw();
		virtual bool		update();
		virtual bool		handleEvent(const sf::Event& event);


	private:
		sf::Sprite			backgroundSprite_;
		sf::Text			pausedText_;
		sf::Text			instructionText_;
};