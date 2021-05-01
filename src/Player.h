#pragma once

#include "Command.h"
#include "InputWrapper.h"

#include <SFML/Window/Event.hpp>


class CommandQueue;

class Player
{
public:
												Player();

	void										assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key							getAssignedKey(Action action) const;

	void										handleEvent(const sf::Event& event, CommandQueue& commands);
	void										handleRealtimeInput(CommandQueue& commands);

private:
	static bool									isRealtimeAction(Action action);
	void										initializeActions();

private:
	std::map<sf::Keyboard::Key, Action>			mKeyBinding;
	std::map<Action, Command>					mActionBinding;
};