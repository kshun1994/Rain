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
		Up				= 1 << 0,
		Down			= 1 << 1,
		Left			= 1 << 2,
		Right			= 1 << 3,
		A				= 1 << 4,
		B				= 1 << 5,
		C				= 1 << 6,
		D				= 1 << 7,
		Start			= 1 << 8,
		Select			= 1 << 9,
	};


public:
												Player();

	void										setPlayerID(int playerID);
	int											getPlayerID() const;

	void										setUsingKeyboard(bool flag);
	bool										isUsingKeyboard() const;

	void										setJoystickID(int joystickID);
	int											getJoystickID() const;
	bool										isUsingAnalogStick() const;
	void										setAnalogThreshold(float threshold);
	void										setUsingAnalogStick(bool flag);
	void										setAnalogXAxis(sf::Joystick::Axis axis);
	void										setAnalogYAxis(sf::Joystick::Axis axis);

	void										assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key							getAssignedKey(Action action) const;

	void										handleEvent(const sf::Event& event, CommandQueue& commands);
	void										handleRealtimeInput(CommandQueue& commands);
	int											getCurrentInputState();
	void										accumulateInput(int input);
	int											getAccumulatedInput() const;
	void										clearAccumulatedInput();

private:
	static bool									isRealtimeAction(Action action);
	void										initializeActions();

	void										cleanInput();

private:
	int											mPlayerID;

	std::map<sf::Keyboard::Key, Action>			mKeyBinding;
	std::map<Action, Command>					mActionBinding;

	bool										mIsUsingKeyboard;
	int											mJoystickID;
	int											mIsUsingAnalogStick;
	float										mAnalogThreshold;
	sf::Joystick::Axis							mAnalogXAxis;
	sf::Joystick::Axis							mAnalogYAxis;

	int											mInputState;
	int											mAccumulatedInput;
};