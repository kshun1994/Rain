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
: playerID_()
, character_(nullptr)
, isUsingKeyboard_(true)
, joystickID_()
, isUsingAnalogStick_(false)
, analogThreshold_(25)
, analogXAxis_(sf::Joystick::X)
, analogYAxis_(sf::Joystick::Y)
, inputState_()
, input_{0}
{
	// Set default key bindings
	keyBinding_[sf::Keyboard::W]		 = Input::Up;
	keyBinding_[sf::Keyboard::S]		 = Input::Down;
	keyBinding_[sf::Keyboard::A]		 = Input::Left;
	keyBinding_[sf::Keyboard::D]		 = Input::Right;
	keyBinding_[sf::Keyboard::J]		 = Input::A;
	keyBinding_[sf::Keyboard::K]		 = Input::B;
	keyBinding_[sf::Keyboard::L]		 = Input::C;
	keyBinding_[sf::Keyboard::SemiColon] = Input::D;
	keyBinding_[sf::Keyboard::Enter]	 = Input::Start;
	keyBinding_[sf::Keyboard::Backspace] = Input::Select;

	// Set initial action bindings
	initializeActions();

	for (auto& pair : actionBinding_)
	{
		pair.second.category = Category::Character;
	}
}

Player::Player(Player::ID PlayerID)
: playerID_(PlayerID)
, character_(nullptr)
, isUsingKeyboard_(true)
, joystickID_()
, isUsingAnalogStick_(false)
, analogThreshold_(50)
, analogXAxis_(sf::Joystick::X)
, analogYAxis_(sf::Joystick::Y)
, inputState_()
, input_{0}
{
	// Set default key bindings
	keyBinding_[sf::Keyboard::W]		 = Input::Up;
	keyBinding_[sf::Keyboard::S]		 = Input::Down;
	keyBinding_[sf::Keyboard::A]		 = Input::Left;
	keyBinding_[sf::Keyboard::D]		 = Input::Right;
	keyBinding_[sf::Keyboard::J]		 = Input::A;
	keyBinding_[sf::Keyboard::K]		 = Input::B;
	keyBinding_[sf::Keyboard::L]		 = Input::C;
	keyBinding_[sf::Keyboard::SemiColon] = Input::D;
	keyBinding_[sf::Keyboard::Enter]	 = Input::Start;
	keyBinding_[sf::Keyboard::Backspace] = Input::Select;

	// Set initial action bindings
	initializeActions();

	for (auto& pair : actionBinding_)
	{
		pair.second.category = Category::Character;
	}
}

void Player::setPlayerID(const Player::ID& PlayerID)
{
	playerID_ = PlayerID;
}

Player::ID Player::getPlayerID() const
{
	return playerID_;
}

void Player::setUsingKeyboard(const bool& flag)
{
	isUsingKeyboard_ = flag;
}

bool Player::isUsingKeyboard() const
{
	return isUsingKeyboard_;
}

void Player::setJoystickID(const int& joystickID)
{
	joystickID_ = joystickID;
}

int Player::getJoystickID() const
{
	return joystickID_;
}

bool Player::isUsingAnalogStick() const
{
	return isUsingAnalogStick_;
}

void Player::setAnalogThreshold(const float& threshold)
{
	analogThreshold_ = threshold;
}

void Player::setUsingAnalogStick(const bool& flag)
{
	isUsingAnalogStick_ = flag;
}

void Player::setAnalogXAxis(const sf::Joystick::Axis& axis)
{
	analogXAxis_ = axis;
}

void Player::setAnalogYAxis(const sf::Joystick::Axis& axis)
{
	analogYAxis_ = axis;
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	for (auto pair : keyBinding_)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			commands.push(actionBinding_[pair.second]);
		}
	}
}

int Player::getCurrentInputState()
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

void Player::accumulateInput(const int& input)
{
	input_ |= inputState_; // Add controller state on current update into accumulated input
	cleanInput(); // Clean accumulated update as according to SOCD
}

int Player::getInput() const
{
	return input_;
}

void Player::cleanInput()
{
	// Clean inputs according to SOCD
	if ((input_ & Input::Left) && (input_ & Input::Right))
	{
		input_ &= ~(Input::Left | Input::Right); // Left + Right = Neutral
	}

	if ((input_ & Input::Up) && (input_ & Input::Down))
	{
		input_ &= ~Input::Down; // Up + Down = Up
	}
}

void Player::clearAccumulatedInput()
{
	input_ = 0;
}


void Player::assignKey(Input input, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = keyBinding_.begin(); itr != keyBinding_.end(); )
	{
		if (itr->second == input)
			keyBinding_.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	keyBinding_[key] = input;
}

sf::Keyboard::Key Player::getAssignedKey(Input input) const
{
	for (auto pair : keyBinding_)
	{
		if (pair.second == input)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
	const float playerSpeed = 200.f;

	//actionBinding_[Action::Input::Left].action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));
	//actionBinding_[Action::Input::Right].action = derivedAction<Character>(CharacterMover(+playerSpeed, 0.f));
}

bool Player::isRealtimeAction(Input input)
{
	switch (input)
	{
	case Input::Up:
	case Input::Down:
	case Input::Left:
	case Input::Right:
		return true;

	default:
		return false;
	}
}