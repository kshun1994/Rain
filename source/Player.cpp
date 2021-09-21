#include "rnpch.h"
#include "Player.h"
#include "CommandQueue.h"
#include "Character.h"


//struct CharacterMover
//{
//	CharacterMover(float vx, float vy)
//		: velocity(vx, vy)
//	{
//	}
//
//	void operator() (Character& character) const
//	{
//		character.accelerate(velocity);
//	}
//
//	sf::Vector2f velocity;
//};

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
, input_{Player::ID::Player1, 0}
{
	// Set default key bindings
	keyBinding_[sf::Keyboard::W]		 = Up;
	keyBinding_[sf::Keyboard::S]		 = Down;
	keyBinding_[sf::Keyboard::A]		 = Left;
	keyBinding_[sf::Keyboard::D]		 = Right;
	keyBinding_[sf::Keyboard::J]		 = A;
	keyBinding_[sf::Keyboard::K]		 = B;
	keyBinding_[sf::Keyboard::L]		 = C;
	keyBinding_[sf::Keyboard::SemiColon] = D;
	keyBinding_[sf::Keyboard::Enter]	 = Start;
	keyBinding_[sf::Keyboard::Backspace] = Select;

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
, input_{PlayerID, 0}
{
	// Set default key bindings
	keyBinding_[sf::Keyboard::W]		 = Up;
	keyBinding_[sf::Keyboard::S]		 = Down;
	keyBinding_[sf::Keyboard::A]		 = Left;
	keyBinding_[sf::Keyboard::D]		 = Right;
	keyBinding_[sf::Keyboard::J]		 = A;
	keyBinding_[sf::Keyboard::K]		 = B;
	keyBinding_[sf::Keyboard::L]		 = C;
	keyBinding_[sf::Keyboard::SemiColon] = D;
	keyBinding_[sf::Keyboard::Enter]	 = Start;
	keyBinding_[sf::Keyboard::Backspace] = Select;

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

unsigned int Player::getCurrentInputState()
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

void Player::accumulateInput(const unsigned int& input)
{
	input_.second |= inputState_; // Add controller state on current update into accumulated input
	cleanInput(); // Clean accumulated update as according to SOCD
}

Player::TaggedInput Player::getInput() const
{
	return input_;
}

Player::GameStats Player::getGameStats() const
{
	return gameStats_;
}

void Player::setGameStats(const std::string& member, const float& value)
{
	// Convert string to lowercase
	std::string lcMember = member;
	std::transform(member.begin(), member.end(), lcMember.begin(), [](unsigned char c) { return std::tolower(c); });

	if (lcMember == "health")
	{
		gameStats_.health = value;
	}
	else if (lcMember == "specialmeter")
	{
		gameStats_.specialMeter = value;
	}
	else
	{
		RN_ERROR("Unknown Player::GameStats lcMember - {}.", lcMember);
	}
}

void Player::cleanInput()
{
	// Clean inputs according to SOCD
	if ((input_.second & Left) && (input_.second & Right))
	{
		input_.second &= ~(Left | Right); // Left + Right = Neutral
	}

	if ((input_.second & Up) && (input_.second & Down))
	{
		input_.second &= ~Down; // Up + Down = Up
	}
}

void Player::clearAccumulatedInput()
{
	input_.second = 0;
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

	//actionBinding_[Action::Left].action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));
	//actionBinding_[Action::Right].action = derivedAction<Character>(CharacterMover(+playerSpeed, 0.f));
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