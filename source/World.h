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

class World : private sf::NonCopyable
{
public:
	typedef std::pair<unsigned int, unsigned int> TaggedInput;

public:
	struct PlayerContext
	{
														PlayerContext() = default;
														PlayerContext(Character* character, 
																	  TaggedInput* rawInput, 
																	  TaggedInput* numpadInput);
														~PlayerContext();
		Character*										Char;
		TaggedInput*									RawInput;
		TaggedInput*									NumpadInput;
	};

	struct BattleContext
	{
														BattleContext() = default;
														BattleContext(PlayerContext* P1, PlayerContext* P2, float* Timer);
														~BattleContext();
		PlayerContext*									P1;
		PlayerContext*									P2;
		float*											Timer;
	};

public:
	explicit											World(sf::RenderWindow& window);
														~World();

	void												update(TaggedInput P1Input, TaggedInput P2Input);
	void												draw();
	CommandQueue&										getCommandQueue();

private:
	void												loadTextures();
	void												buildScene();

	void												adaptPlayerPosition();

	TaggedInput											translateToNumpadInput(TaggedInput playerRawInput);

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
	CommandQueue										mCommandQueue;

	sf::FloatRect										mWorldBounds;
	sf::Vector2f										mSpawnPosition;
	float												mScrollSpeed;

	float												mTimer;

	Character*											mP1Character;
	Character*											mP2Character;

	TaggedInput*										mP1RawInput;
	TaggedInput*										mP2RawInput;
	TaggedInput*										mP1NumpadInput;
	TaggedInput*										mP2NumpadInput;

	std::vector<int>									inputs;
	unsigned int										mDebugPrevInput;

	std::vector<std::unique_ptr<InputTrigger>>			mTriggerArray;
};