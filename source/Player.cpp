#include "rnpch.h"
#include "Player.h"
#include "CommandQueue.h"
#include "Character.h"


struct CharacterMover
{
	CharacterMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Character& character) const
	{
		character.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

Player::Player()
: mPlayerID(0)
, mIsUsingKeyboard(true)
, mJoystickID()
, mIsUsingAnalogStick(false)
, mAnalogThreshold(25)
, mAnalogXAxis(sf::Joystick::X)
, mAnalogYAxis(sf::Joystick::Y)
, mInputState()
, mAccumulatedInput()
{
	// Set default key bindings
	mKeyBinding[sf::Keyboard::W]		 = Up;
	mKeyBinding[sf::Keyboard::S]		 = Down;
	mKeyBinding[sf::Keyboard::A]		 = Left;
	mKeyBinding[sf::Keyboard::D]		 = Right;
	mKeyBinding[sf::Keyboard::J]		 = A;
	mKeyBinding[sf::Keyboard::K]		 = B;
	mKeyBinding[sf::Keyboard::L]		 = C;
	mKeyBinding[sf::Keyboard::SemiColon] = D;
	mKeyBinding[sf::Keyboard::Enter]	 = Start;
	mKeyBinding[sf::Keyboard::Backspace] = Select;

	// Set initial action bindings
	initializeActions();

	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::Character;
	}
}

void Player::setPlayerID(int playerID)
{
	mPlayerID = playerID;
}

int Player::getPlayerID() const
{
	return mPlayerID;
}

void Player::setUsingKeyboard(bool flag)
{
	mIsUsingKeyboard = flag;
}

bool Player::isUsingKeyboard() const
{
	return mIsUsingKeyboard;
}

void Player::setJoystickID(int joystickID)
{
	mJoystickID = joystickID;
}

int Player::getJoystickID() const
{
	return mJoystickID;
}

bool Player::isUsingAnalogStick() const
{
	return mIsUsingAnalogStick;
}

void Player::setAnalogThreshold(float threshold)
{
	mAnalogThreshold = threshold;
}

void Player::setUsingAnalogStick(bool flag)
{
	mIsUsingAnalogStick = flag;
}

void Player::setAnalogXAxis(sf::Joystick::Axis axis)
{
	mAnalogXAxis = axis;
}

void Player::setAnalogYAxis(sf::Joystick::Axis axis)
{
	mAnalogYAxis = axis;
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	for (auto pair : mKeyBinding)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			commands.push(mActionBinding[pair.second]);
		}
	}
}

unsigned int Player::getCurrentInputState()
{
	mInputState = 0;

	if (mIsUsingKeyboard)
	{
		for (auto pair : mKeyBinding)
		{
			if (sf::Keyboard::isKeyPressed(pair.first))
			{
				mInputState |= pair.second;
			}
		}
	}
	else // If using a joystick
	{
		if (mIsUsingAnalogStick)
		{
			if (sf::Joystick::getAxisPosition(mJoystickID, mAnalogYAxis) > mAnalogThreshold)
			{
				mInputState |= Action::Up;
			}
			else if (sf::Joystick::getAxisPosition(mJoystickID, mAnalogYAxis) < -mAnalogThreshold)
			{
				mInputState |= Action::Down;
			}

			if (sf::Joystick::getAxisPosition(mJoystickID, mAnalogXAxis) > mAnalogThreshold)
			{
				mInputState |= Action::Right;
			}
			else if (sf::Joystick::getAxisPosition(mJoystickID, mAnalogXAxis) < -mAnalogThreshold)
			{
				mInputState |= Action::Left;
			}

		}

		for (auto pair : mKeyBinding)
		{
			if (sf::Joystick::isButtonPressed(mJoystickID, pair.first))
			{
				mInputState |= pair.second;
			}
		}
	}

	return mInputState;
}

void Player::accumulateInput(unsigned int input)
{
	mAccumulatedInput |= mInputState; // Add controller state on current update into accumulated input
	cleanInput(); // Clean accumulated update as according to SOCD
}

unsigned int Player::getAccumulatedInput() const
{
	return mAccumulatedInput;
}

void Player::clearAccumulatedInput()
{
	mAccumulatedInput = 0;
}

void Player::cleanInput()
{
	// Clean inputs according to SOCD
	if ((mAccumulatedInput & Left) && (mAccumulatedInput & Right))
	{
		mAccumulatedInput &= ~(Left | Right); // Left + Right = Neutral
	}

	if ((mAccumulatedInput & Up) && (mAccumulatedInput & Down))
	{
		mAccumulatedInput &= ~Down; // Up + Down = Up
	}
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	const float playerSpeed = 200.f;

	mActionBinding[Action::Left].action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));
	mActionBinding[Action::Right].action = derivedAction<Character>(CharacterMover(+playerSpeed, 0.f));
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
	case Up:
	case Down:
	case Left:
	case Right:
		return true;

	default:
		return false;
	}
}