#include <World.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

const int StageWidth = 2400;
const int StageHeight = 1008;

const float ViewYOffset = 400;


World::World(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, StageWidth, StageHeight)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height)
	, mScrollSpeed(-50.f)
	, mPlayerCharacter(nullptr)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition.x, mSpawnPosition.y - ViewYOffset);
}

void World::loadTextures()
{
	mTextures.load(Textures::Enkidu,		"Media/Texture/Enkidu/000.png");
	mTextures.load(Textures::Yuzuriha,		"Media/Texture/Yuzuriha/000.png");
	mTextures.load(Textures::StageMomiji,	"Media/Texture/_Stage/MomijiShrineScaledx3.png");
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i) // iterate thorugh layer node pointers
	{
		SceneNode::Ptr layer(new SceneNode()); // declare new SceneNode for current element
		mSceneLayers[i] = layer.get(); // std::unique_ptr::get() returns pointer to stored object, no ownership transferred to array

		mSceneGraph.attachChild(std::move(layer)); // attach the newest node to scene graph's root node
	}

	sf::Texture& texture = mTextures.get(Textures::StageMomiji);
	sf::IntRect textureRect(mWorldBounds);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	std::unique_ptr<Character> enkidu(new Character(Character::Enkidu, mTextures));
	mPlayerCharacter = enkidu.get();
	mPlayerCharacter->setPosition(mSpawnPosition);
	mPlayerCharacter->setVelocity(200.f, 0.f);
	mSceneLayers[Characters]->attachChild(std::move(enkidu));
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::update(sf::Time dt)
{
	// Move the player sidewards (plane scouts follow the main aircraft)
	sf::Vector2f position = mPlayerCharacter->getPosition();
	sf::Vector2f velocity = mPlayerCharacter->getVelocity();

	// If player touches borders, flip its X velocity
	if (position.x <= mWorldBounds.left + 635.f
		|| position.x >= mWorldBounds.left + mWorldBounds.width - 635.f)
	{
		velocity.x = -velocity.x;
		mPlayerCharacter->setVelocity(velocity);
	}

	mWorldView.setCenter(position.x, position.y - ViewYOffset);
	mSceneGraph.update(dt);
}