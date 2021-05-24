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

struct PlayerContext
{
};

Player::Player()
: mPlayerID()
, mCharacter(nullptr)
, mIsUsingKeyboard(true)
, mJoystickID()
, mIsUsingAnalogStick(false)
, mAnalogThreshold(25)
, mAnalogXAxis(sf::Joystick::X)
, mAnalogYAxis(sf::Joystick::Y)
, mInputState()
, mInput{Player::ID::Player1, 0}
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

Player::Player(Player::ID PlayerID)
: mPlayerID(PlayerID)
, mCharacter(nullptr)
, mIsUsingKeyboard(true)
, mJoystickID()
, mIsUsingAnalogStick(false)
, mAnalogThreshold(50)
, mAnalogXAxis(sf::Joystick::X)
, mAnalogYAxis(sf::Joystick::Y)
, mInputState()
, mInput{PlayerID, 0}
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

void Player::setPlayerID(const Player::ID& PlayerID)
{
	mPlayerID = PlayerID;
}

Player::ID Player::getPlayerID() const
{
	return mPlayerID;
}

void Player::setUsingKeyboard(const bool& flag)
{
	mIsUsingKeyboard = flag;
}

bool Player::isUsingKeyboard() const
{
	return mIsUsingKeyboard;
}

void Player::setJoystickID(const int& joystickID)
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

void Player::setAnalogThreshold(const float& threshold)
{
	mAnalogThreshold = threshold;
}

void Player::setUsingAnalogStick(const bool& flag)
{
	mIsUsingAnalogStick = flag;
}

void Player::setAnalogXAxis(const sf::Joystick::Axis& axis)
{
	mAnalogXAxis = axis;
}

void Player::setAnalogYAxis(const sf::Joystick::Axis& axis)
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
				mInputState |= Input::Up;
			}
			else if (sf::Joystick::getAxisPosition(mJoystickID, mAnalogYAxis) < -mAnalogThreshold)
			{
				mInputState |= Input::Down;
			}

			if (sf::Joystick::getAxisPosition(mJoystickID, mAnalogXAxis) > mAnalogThreshold)
			{
				mInputState |= Input::Right;
			}
			else if (sf::Joystick::getAxisPosition(mJoystickID, mAnalogXAxis) < -mAnalogThreshold)
			{
				mInputState |= Input::Left;
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

void Player::accumulateInput(const unsigned int& input)
{
	mInput.second |= mInputState; // Add controller state on current update into accumulated input
	cleanInput(); // Clean accumulated update as according to SOCD
}

Player::TaggedInput Player::getInput() const
{
	return mInput;
}

void Player::cleanInput()
{
	// Clean inputs according to SOCD
	if ((mInput.second & Left) && (mInput.second & Right))
	{
		mInput.second &= ~(Left | Right); // Left + Right = Neutral
	}

	if ((mInput.second & Up) && (mInput.second & Down))
	{
		mInput.second &= ~Down; // Up + Down = Up
	}
}

void Player::clearAccumulatedInput()
{
	mInput.second = 0;
}


void Player::assignKey(Input input, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == input)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = input;
}

sf::Keyboard::Key Player::getAssignedKey(Input input) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == input)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	const float playerSpeed = 200.f;

	//mActionBinding[Action::Left].action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));
	//mActionBinding[Action::Right].action = derivedAction<Character>(CharacterMover(+playerSpeed, 0.f));
}

bool Player::isRealtimeAction(Input input)
{
	switch (input)
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