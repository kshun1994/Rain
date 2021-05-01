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

	void operator() (Character& character, sf::Time) const
	{
		character.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

Player::Player()
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

	// Set initial action bindings
	initializeActions();

	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::Character;
	}
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

	mActionBinding[Left].action = derivedAction<Character>(CharacterMover(-playerSpeed, 0.f));
	mActionBinding[Right].action = derivedAction<Character>(CharacterMover(+playerSpeed, 0.f));
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