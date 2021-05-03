#pragma once

#include "Command.h"
#include "InputWrapper.h"

#include <SFML/Window/Event.hpp>


class CommandQueue;

class Player
{
public:
	enum Action
	{
		Up				= 1,
		Down			= 2,
		Left			= 4,
		Right			= 8,
		A				= 16,
		B				= 32,
		C				= 64,
		D				= 128,
		Start			= 256,
		Select			= 512,
	};

public:
												Player();

	void										setPlayerID(int playerID);
	int											getPlayerID() const;

	void										setUsingKeyboard(bool flag);
	bool										isUsingKeyboard() const;

	void										setJoystickID(int joystickID);
	int											getJoystickID() const;

	void										assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key							getAssignedKey(Action action) const;

	void										handleEvent(const sf::Event& event, CommandQueue& commands);
	void										handleRealtimeInput(CommandQueue& commands);

private:
	static bool									isRealtimeAction(Action action);
	void										initializeActions();
	void										interpretInputState();

private:
	int											mPlayerID;

	std::map<sf::Keyboard::Key, Action>			mKeyBinding;
	std::map<Action, Command>					mActionBinding;

	bool										mIsUsingKeyboard;
	int											mJoystickID;

	int											mInputState;
};