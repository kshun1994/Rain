#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Character.h"
#include "CommandQueue.h"
#include "InputTrigger.h"
#include "Player.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
	explicit											World(sf::RenderWindow& window);
														~World();

	void												update(Player::TaggedInput player1Input, Player::TaggedInput player2Input);
	void												draw();
	CommandQueue&										getCommandQueue();

private:
	void												loadTextures();
	void												buildScene();

	void												adaptPlayerPosition();

	Player::TaggedInput									translateToNumpadInput(Player::TaggedInput playerInput);
	void												updateInputBuffer(Player::TaggedInput numpadInput, std::pair<Player::ID, std::deque<unsigned int>> &inputBuffer);
	
private:
	enum Layer
	{
		Background,
		Characters,
		LayerCount,
	};

private:
	sf::RenderWindow&									mWindow;
	sf::View											mWorldView;
	TextureHolder										mTextures;
	SceneNode											mSceneGraph;
	std::array<SceneNode*, LayerCount>					mSceneLayers;
	CommandQueue										mCommandQueue;

	sf::FloatRect										mWorldBounds;
	sf::Vector2f										mSpawnPosition;
	float												mScrollSpeed;

	Character*											mP1Character;
	Character*											mP2Character;

	std::pair<Player::ID, std::deque<unsigned int>>		mP1InputBuffer;
	std::pair<Player::ID, std::deque<unsigned int>>		mP2InputBuffer;

	std::vector<int>									inputs;
	unsigned int										mDebugPrevInput;

	std::vector<std::unique_ptr<InputTrigger>>			mTriggerArray;
};