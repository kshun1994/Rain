#include "rnpch.h"
#include "World.h"

#include "Action.h"


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
	, mP1Character(nullptr)
	, mP2Character(nullptr)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition.x, mSpawnPosition.y - ViewYOffset);

	// TEST STUFF
	// Initialize input triggers
	std::vector<std::vector<unsigned int>> inputs =
	{
		{ 2, 3, 6 },	
		{ 6, 2, 3 },	
		{ 2, 1, 4 },	
		{ 4, 2, 1 },	
		{ 4, 1, 2, 3, 6},
		{ 6, 3, 2, 1, 4},
		{ 2, 5, 2 },	
	};

	std::vector<unsigned int> buffers =
	{
		CONST_BUFFER_236,
		CONST_BUFFER_623,
		CONST_BUFFER_214,
		CONST_BUFFER_421,
		CONST_BUFFER_HCF,
		CONST_BUFFER_HCB,
		CONST_BUFFER_22,
	};

	for (int i = 0; i < inputs.size(); i++)
	{
		mTriggerArray.push_back(std::make_unique<InputTrigger>());
		mTriggerArray[i]->setMotion(inputs[i]);
		mTriggerArray[i]->setBuffer(buffers[i]);
	}
}

World::~World()
{
}

void World::loadTextures()
{
	mTextures.load(Textures::ID::Enkidu,		"media/texture/enkidu/Enkidu_idle.png");
	mTextures.load(Textures::ID::Yuzuriha,		"media/texture/enkidu/aok0000.png");
	mTextures.load(Textures::ID::Shun,			"media/texture/enkidu/shun_design.png");
	mTextures.load(Textures::ID::StageMomiji,	"media/texture/_stage/MomijiShrineScaledx3.png");
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i) // iterate thorugh layer node pointers
	{
		SceneNode::Ptr layer(new SceneNode()); // declare new SceneNode for current element
		mSceneLayers[i] = layer.get(); // std::unique_ptr::get() returns pointer to stored object, no ownership transferred to array

		mSceneGraph.attachChild(std::move(layer)); // attach the newest node to scene graph's root node
	}

	sf::Texture& texture = mTextures.get(Textures::ID::StageMomiji);
	sf::IntRect textureRect(mWorldBounds);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	//std::unique_ptr<Character> aokoTest(new Character(Character::Yuzuriha, mTextures));
	//mP1Character = aokoTest.get();
	//mP1Character->setScale(sf::Vector2f(3.7f, 3.7f));
	//mP1Character->setPosition(mSpawnPosition.x, mSpawnPosition.y + 50);
	//mSceneLayers[Characters]->attachChild(std::move(aokoTest));

	std::unique_ptr<Character> enkidu(new Character(Character::Enkidu, mTextures));
	mP1Character = enkidu.get();
	mP1Character->setPosition(mSpawnPosition.x, mSpawnPosition.y - 50);
	mSceneLayers[Characters]->attachChild(std::move(enkidu));

	//std::unique_ptr<Character> shun(new Character(Character::Shun, mTextures));
	//mP1Character = shun.get();
	//mP1Character->setScale(sf::Vector2f(5.0f, 5.0f));
	//mP1Character->setPosition(mSpawnPosition.x, mSpawnPosition.y);
	//mSceneLayers[Characters]->attachChild(std::move(shun));
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

std::vector<std::string> inputString =
{
	"quarter-circle forward",
	"dragon punch",
	"quarter-circle backward",
	"reverse dragon punch",
	"half-circle forward",
	"half-circle backward",
	"down-down",
};

void World::update(unsigned int player1Input, unsigned int player2Input)
{
	// TODO: should character facings be updated before or after input buffers are updated?

	// Read in accumulated player input for current update and add to input buffer
	updateInputBuffer(translateToNumpadInput(player1Input), mP1InputBuffer);
	updateInputBuffer(translateToNumpadInput(player2Input), mP2InputBuffer);

	// Testing triggers
	for (int i = 0; i < mTriggerArray.size(); i++)
	{
		mTriggerArray[i]->update(mP1InputBuffer.back());
		if (mTriggerArray[i]->isTriggered())
		{
			RN_DEBUG("Motion input detected -- {}", inputString[i]);
		}
	}

	// Check hitbox/hurtbox overlaps

	// Check if player characters are actionable

		// If actionable, initiate action based on input buffer readout



	sf::Vector2f position = mP1Character->getPosition();

	mP1Character->setVelocity(0.f, 0.f);

	// Forward commands to scene graph
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop());
	}

	mSceneGraph.update();
	adaptPlayerPosition();
	mWorldView.setCenter(mP1Character->getPosition().x, 
						 mP1Character->getPosition().y - ViewYOffset);

	// RN_DEBUG("Current character coordinates are: ({0}, {1}).", 
	// 	mP1Character->getPosition().x, mP1Character->getPosition().y);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = mP1Character->getPosition();
	position.x = std::max(position.x, mWorldBounds.left + borderDistance);
	position.x = std::min(position.x, mWorldBounds.left + mWorldBounds.width - borderDistance);
	mP1Character->setPosition(position);
}

// TODO: need a function to update character facings

#ifdef RN_DEBUG
unsigned int prevInput = 0;
#endif // RN_DEBUG

unsigned int World::translateToNumpadInput(unsigned int playerInput)
{
	// Change bit flag inputs from Player to numpad notation. Keep bit flags for buttons (A = 1 << 4 = 16 etc.). Since numpad 
	// notation doesn't go past 9, the entire numpad + buttons input can be stored as a single int.

	unsigned int numpad = 5; // Neutral

	// If x-axis input matches current character facing
	if ((playerInput & (Action::Left | Action::Right)) == mP1Character->getFacing())
	{
		numpad += 1;
	}
	// If x-axis input is opposite of current character facing
	if ((playerInput & (Action::Left | Action::Right)) == (mP1Character->getFacing() ^ Action::Left ^ Action::Right)) // Flip the bits on character facing to get opposite
	{
		numpad -= 1;
	}
	// If upward input
	if (playerInput & Action::Up)
	{
		numpad += 3;
	}
	// If downward input
	if (playerInput & Action::Down)
	{
		numpad -= 3;
	}

	if (numpad != prevInput)
	{
		RN_DEBUG("Input (Numpad) -- {}", numpad);
		prevInput = numpad;
		//RN_DEBUG("Full input ------ {}", numpad + (playerInput >> 4 << 4));
	}

	return numpad + (playerInput >> 4 << 4); // Convert the first four bits in playerInput to 0s; preserve bits pertaining to buttons (fifth onward)
}

void World::updateInputBuffer(unsigned int numpadInput, std::deque<unsigned int> &inputBuffer)
{
	inputBuffer.push_back(numpadInput);
	while (inputBuffer.size() > CONST_MAX_INPUT_BUFFER)
	{
		inputBuffer.pop_front();
	}
}