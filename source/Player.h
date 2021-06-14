#pragma once

#include "Command.h"
#include "InputWrapper.h"
#include "Input.h"

#include <SFML/Window/Event.hpp>


class CommandQueue;
class Character;

class Player
{
public:
	enum ID
	{
		Player1 = 0,
		Player2 = 1,
	};

public:
	typedef std::pair<unsigned int, unsigned int> TaggedInput;

public:
												Player();
												Player(ID PlayerID);

	void										setPlayerID(const ID& PlayerID);
	ID											getPlayerID() const;

	void										setUsingKeyboard(const bool& flag);
	bool										isUsingKeyboard() const;

	void										setJoystickID(const int& joystickID);
	int											getJoystickID() const;
	bool										isUsingAnalogStick() const;
	void										setAnalogThreshold(const float& threshold);
	void										setUsingAnalogStick(const bool& flag);
	void										setAnalogXAxis(const sf::Joystick::Axis& axis);
	void										setAnalogYAxis(const sf::Joystick::Axis& axis);

	void										assignKey(Input input, sf::Keyboard::Key key);
	sf::Keyboard::Key							getAssignedKey(Input input) const;

	void										handleRealtimeInput(CommandQueue& commands);
	unsigned int								getCurrentInputState();
	void										accumulateInput(const unsigned int& input);
	void										clearAccumulatedInput();

	TaggedInput									getInput() const;

private:
	static bool									isRealtimeAction(Input input);
	void										initializeActions();

	void										cleanInput();

private:
	ID											playerID_;
	Character*									character_;

	std::map<sf::Keyboard::Key, Input>			keyBinding_;
	std::map<Input, Command>					actionBinding_;

	bool										isUsingKeyboard_;
	int											joystickID_;
	int											isUsingAnalogStick_;
	float										analogThreshold_;
	sf::Joystick::Axis							analogXAxis_;
	sf::Joystick::Axis							analogYAxis_;

	unsigned int								inputState_;
	TaggedInput									input_;
};