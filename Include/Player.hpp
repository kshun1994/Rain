#pragma once

#include <Command.hpp>

#include <SFML/Window/Event.hpp>

#include <map>


class CommandQueue;

class Player
{
public:
	enum Action
	{
		Up,
		Down,
		Left,
		Right,
		A,
		B,
		C,
		D,
		ActionCount,
	};

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