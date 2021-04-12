#include <Player.hpp>
#include <Character.hpp>

Player::Player()
{
	mKeyBinding[sf::Keyboard::A] = InputLeft;
	mKeyBinding[sf::Keyboard::D] = InputRight;

	mActionBinding[InputLeft].action = [](SceneNode& node, sf::Time dt)
	{
		node.move(-PlayerSpeed * dt.asSeconds(), 0.f);
	};

	mActionBinding[InputRight].action = [](SceneNode& node, sf::Time dt)
	{
		node.move(PlayerSpeed * dt.asSeconds(), 0.f);
	};

	for (auto& pair : mActionBinding)
	{
		pair.second.category = Category::PlayerCharacter;
	}
}

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

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	//if (event.type == sf::Event::KeyPressed)
	//{
	//	// Check if pressed key appears in key binding, trigger command if so
	//	auto found = mKeyBinding.find(event.key.code);
	//	if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
	//		commands.push(mActionBinding[found->second]);
	//}
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