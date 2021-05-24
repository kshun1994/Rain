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
	typedef std::pair<unsigned int, unsigned int> TaggedInput;

public:
	struct PlayerObjects
	{
														PlayerObjects() = default;

		Player&											mPlayer;
		Character										mCharacter;
	};

	struct BattleContext
	{
														BattleContext() = default;
														~BattleContext();
		float*											Timer;
	};

public:
	explicit											World(sf::RenderWindow& window, Player& P1, Player& P2);
														~World();

	void												update();
	void												draw();

private:
	void												loadTextures();
	void												buildScene();

	void												adaptPlayerPosition();
	void												adaptPlayerFacing();

	TaggedInput											translateToNumpadInput(const TaggedInput& playerRawInput);

private:
	enum Layer
	{
		Background,
		Player1,
		Player2,
		UI,
		LayerCount,
	};

private:
	sf::RenderWindow&									mWindow;
	sf::View											mWorldView;
	TextureHolder										mTextures;
	SceneNode											mSceneGraph;
	std::array<SceneNode*, LayerCount>					mSceneLayers;

	sf::FloatRect										mWorldBounds;
	sf::Vector2f										mSpawnPosition;

	float												mTimer;

	Player&												mP1;
	Player&												mP2;

	std::shared_ptr<Character>							mP1Char;
	std::shared_ptr<Character>							mP2Char;
	std::array<std::shared_ptr<Character>, 2>			mCharArray;

	std::vector<int>									inputs;
	unsigned int										mDebugPrevInput;

	std::vector<std::unique_ptr<InputTrigger>>			mTriggerArray;
};