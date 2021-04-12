#include <World.hpp>

#include <SFML/Graphics/RenderWindow.hpp>


World::World(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height)
	, mScrollSpeed(-50.f)
	, mPlayerCharacter(nullptr)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::loadTextures()
{
	mTextures.load(Textures::Enkidu,		"Media/Texture/Enkidu/000.png");
	mTextures.load(Textures::Yuzuriha,		"Media/Texture/Yuzuriha/000.png");
	mTextures.load(Textures::StageMomiji,	"Media/Texture/_Stage/MomijiShrine.png");
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i) // iterate thorugh layer node pointers
	{
		SceneNode::Ptr layer(new SceneNode()); // declare new SceneNode for current element
		mSceneLayers[i] = layer.get(); // std::unique_ptr::get() returns pointer to stored object, no ownership transferred to array

		mSceneGraph.attachChild(std::move(layer)); // attach the newest node to scene graph's root node
	}


}