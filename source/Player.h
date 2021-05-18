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
		Player1 = 1,
		Player2 = 2,
	};

public:
	typedef std::pair<Player::ID, unsigned int> TaggedInput;

public:
												Player();
												Player(ID PlayerID);

	void										setPlayerID(ID PlayerID);
	ID											getPlayerID() const;

	void										setUsingKeyboard(bool flag);
	bool										isUsingKeyboard() const;

	void										setJoystickID(int joystickID);
	int											getJoystickID() const;
	bool										isUsingAnalogStick() const;
	void										setAnalogThreshold(float threshold);
	void										setUsingAnalogStick(bool flag);
	void										setAnalogXAxis(sf::Joystick::Axis axis);
	void										setAnalogYAxis(sf::Joystick::Axis axis);

	void										assignKey(Input input, sf::Keyboard::Key key);
	sf::Keyboard::Key							getAssignedKey(Input input) const;

	void										handleRealtimeInput(CommandQueue& commands);
	unsigned int								getCurrentInputState();
	void										accumulateInput(unsigned int input);
	void										clearAccumulatedInput();

	TaggedInput									getInput() const;

private:
	static bool									isRealtimeAction(Input input);
	void										initializeActions();

	void										cleanInput();
	unsigned int								translateToNumpadInput(unsigned int playerInput);

private:
	ID											mPlayerID;
	Character*									mCharacter;

	std::map<sf::Keyboard::Key, Input>			mKeyBinding;
	std::map<Input, Command>					mActionBinding;

	bool										mIsUsingKeyboard;
	int											mJoystickID;
	int											mIsUsingAnalogStick;
	float										mAnalogThreshold;
	sf::Joystick::Axis							mAnalogXAxis;
	sf::Joystick::Axis							mAnalogYAxis;

	unsigned int								mInputState;
	TaggedInput									mInput;
};