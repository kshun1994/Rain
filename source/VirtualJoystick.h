#pragma once
#include "Input.h"

#include <SFML/Window/Event.hpp>

class VirtualJoystick
{
public:
							VirtualJoystick();
							~VirtualJoystick();

	void					setUsingKeyboard(const bool& flag);
	bool					isUsingKeyboard() const;

	void					setJoystickID(const int& joystickID);
	int						getJoystickID() const;

	void					setUsingAnalogStick(const bool& flag);
	bool					isUsingAnalogStick() const;
	void					setAnalogThreshold(const float& threshold);

	void					setAnalogXAxis(const sf::Joystick::Axis& axis);
	void					setAnalogYAxis(const sf::Joystick::Axis& axis);

	unsigned int			getCurrentInputState();
	void					accumulateInput(const unsigned int& input);
	void					clearAccumulatedInput();

private:
	void					cleanInput();

private:
	int						joystickID_;

	bool					isUsingKeyboard_;

	int						isUsingAnalogStick_;

	float					analogThreshold_;
	sf::Joystick::Axis		analogXAxis_;
	sf::Joystick::Axis		analogYAxis_;

	unsigned int			inputState_;
};

