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
	virtual bool																update(unsigned int player1Input, unsigned int player2Input);
	virtual bool																handleEvent(const sf::Event& event);

private:
	void																		updateLabels();
	void																		addButtonLabel(Action action, float y, const std::string& text, Context context);

private:
	sf::Sprite																	mBackgroundSprite;
	GUI::Container																mGUIContainer;
	std::map<Action, GUI::Button::Ptr>									mBindingButtons;
	std::map<Action, GUI::Label::Ptr>									mBindingLabels;
};
