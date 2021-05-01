#pragma once

#include <SFML/Graphics.hpp>

class InputWrapper
{
public:
	enum class Action
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
		ActionCount,
	};

public:
	void								setKeyboard();
	void								setJoystickID(sf::Joystick joystick);
	void								assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key					getAssignedKey(Action action);

private:
	void								initializeInputs();

private:
	bool								mIsUsingKeyboard;
	sf::Joystick						mJoystick;
	sf::Keyboard						mKeyboard;
	int									mInput;

};

