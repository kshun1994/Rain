#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Character.h"
#include "CommandQueue.h"
#include "InputTrigger.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

struct InputElement
{
	int							Input;
	int							Duration;
};

class World : private sf::NonCopyable
{
public:
	enum class Numpad
	{
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
	};

public:
	explicit											World(sf::RenderWindow& window);
														~World();

	void												update(unsigned int player1Input, unsigned int player2Input);
	void												draw();
	CommandQueue&										getCommandQueue();

private:
	void												loadTextures();
	void												buildScene();

	void												adaptPlayerPosition();

	unsigned int										translateToNumpadInput(unsigned int playerInput);
	void												updateInputBuffer(unsigned int numpadInput, std::deque<unsigned int> &inputBuffer);
	
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

	std::deque<unsigned int>							mP1InputBuffer;
	std::deque<unsigned int>							mP2InputBuffer;

	std::vector<int>									inputs;

	std::vector<std::unique_ptr<InputTrigger>>			mTriggerArray;
};