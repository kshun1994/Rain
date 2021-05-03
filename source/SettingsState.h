#pragma once

#include "State.h"
#include "Player.h"
#include "Container.h"
#include "Button.h"
#include "Label.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class SettingsState : public State
{
public:
																				SettingsState(StateStack& stack, Context context);
	virtual void																draw();
	virtual bool																update();
	virtual bool																handleEvent(const sf::Event& event);

private:
	void																		updateLabels();
	void																		addButtonLabel(Player::Action action, float y, const std::string& text, Context context);

private:
	sf::Sprite																	mBackgroundSprite;
	GUI::Container																mGUIContainer;
	std::array<GUI::Button::Ptr, magic_enum::enum_count<Player::Action>()>		mBindingButtons;
	std::array<GUI::Label::Ptr, magic_enum::enum_count<Player::Action>()>		mBindingLabels;
};
