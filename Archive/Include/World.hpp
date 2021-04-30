#pragma once

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>
#include <SpriteNode.hpp>
#include <Character.hpp>
#include <CommandQueue.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
	explicit											World(sf::RenderWindow& window);
	void												update(sf::Time dt);
	void												draw();
	CommandQueue&										getCommandQueue();

private:
	void												loadTextures();
	void												buildScene();
	void												adaptPlayerPosition();

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
	Character*											mPlayerCharacter;
};