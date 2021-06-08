#include "VirtualJoystick.h"

void VirtualJoystick::setUsingKeyboard(const bool& flag)
{
	isUsingKeyboard_ = flag;
}

bool VirtualJoystick::isUsingKeyboard() const
{
	return isUsingKeyboard_;
}

void VirtualJoystick::setJoystickID(const int& joystickID)
{
	joystickID_ = joystickID;
}

int VirtualJoystick::getJoystickID() const
{
	return joystickID_;
}

void VirtualJoystick::setUsingAnalogStick(const bool& flag)
{
	isUsingAnalogStick_ = flag;
}

bool VirtualJoystick::isUsingAnalogStick() const
{
	return isUsingAnalogStick_;
}

void VirtualJoystick::setAnalogThreshold(const float& threshold)
{
	analogThreshold_ = threshold;
}

void VirtualJoystick::setAnalogXAxis(const sf::Joystick::Axis& axis)
{
	analogXAxis_ = axis;
}

void VirtualJoystick::setAnalogYAxis(const sf::Joystick::Axis& axis)
{
	analogYAxis_ = axis;
}

unsigned int VirtualJoystick::getCurrentInputState()
{
	inputState_ = 0;

	if (isUsingKeyboard_)
	{
		for (auto pair : keyBinding_)
		{
			if (sf::Keyboard::isKeyPressed(pair.first))
			{
				inputState_ |= pair.second;
			}
		}
	}
	else // If using a joystick
	{
		if (isUsingAnalogStick_)
		{
			if (sf::Joystick::getAxisPosition(joystickID_, analogYAxis_) > analogThreshold_)
			{
				inputState_ |= Input::Up;
			}
			else if (sf::Joystick::getAxisPosition(joystickID_, analogYAxis_) < -analogThreshold_)
			{
				inputState_ |= Input::Down;
			}

			if (sf::Joystick::getAxisPosition(joystickID_, analogXAxis_) > analogThreshold_)
			{
				inputState_ |= Input::Right;
			}
			else if (sf::Joystick::getAxisPosition(joystickID_, analogXAxis_) < -analogThreshold_)
			{
				inputState_ |= Input::Left;
			}

		}

		for (auto pair : keyBinding_)
		{
			if (sf::Joystick::isButtonPressed(joystickID_, pair.first))
			{
				inputState_ |= pair.second;
			}
		}
	}

	return inputState_;
}

void VirtualJoystick::accumulateInput(const unsigned int& input)
{
	virtualJoystick_ |= inputState_; // Add controller state on current update into accumulated input
	cleanInput(); // Clean accumulated update as according to SOCD
}

unsigned int VirtualJoystick::getVirtualJoystick() const
{
	return virtualJoystick_;
}

void VirtualJoystick::cleanInput()
{
	// Clean inputs according to SOCD
	if ((virtualJoystick_ & Left) && (virtualJoystick_ & Right))
	{
		virtualJoystick_ &= ~(Left | Right); // Left + Right = Neutral
	}

	if ((virtualJoystick_ & Up) && (virtualJoystick_ & Down))
	{
		virtualJoystick_ &= ~Down; // Up + Down = Up
	}
}

void VirtualJoystick::clearAccumulatedInput()
{
	virtualJoystick_ = 0;
}
