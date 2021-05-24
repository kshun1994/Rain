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
	void																		addButtonLabel(Input input, float y, const std::string& text, Context context);

private:
	sf::Sprite																	backgroundSprite_;
	GUI::Container																gUIContainer_;
	std::map<Input, GUI::Button::Ptr>									bindingButtons_;
	std::map<Input, GUI::Label::Ptr>									bindingLabels_;
};
