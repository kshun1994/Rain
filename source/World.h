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

		Player&											player_;
		Character										character_;
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
	sf::RenderWindow&									window_;
	sf::View											worldView_;
	TextureHolder										textures_;
	SceneNode											sceneGraph_;
	std::array<SceneNode*, LayerCount>					sceneLayers_;

	sf::FloatRect										worldBounds_;
	sf::Vector2f										spawnPosition_;

	float												timer_;

	Player&												p1_;
	Player&												p2_;

	std::shared_ptr<Character>							p1Char_;
	std::shared_ptr<Character>							p2Char_;
	std::array<std::shared_ptr<Character>, 2>			charArray_;

	std::vector<int>									inputs;
	unsigned int										debugPrevInput_;

	std::vector<std::unique_ptr<InputTrigger>>			triggerArray_;
};