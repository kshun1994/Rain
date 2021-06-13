#include "rnpch.h"
#include "World.h"

#include "Input.h"


const int StageWidth = 2400;
const int StageHeight = 1008;

const float ViewYOffset = 400;

World::World(sf::RenderWindow& window, Player& P1, Player& P2)
: window_(window)
, worldView_(window.getDefaultView())
, textures_()
, sceneGraph_()
, sceneLayers_()
, worldBounds_(0.f, 0.f, StageWidth, StageHeight)
, spawnPosition_(worldView_.getSize().x / 2.f, worldBounds_.height)
, p1_(P1)
, p2_(P2)
, timer_()
, debugPrevInput_(0)
{
	// Need some step where input devices assigned in preceding Device Select State are assigned to players

	loadTextures();
	buildScene();

	// Prepare the view
	worldView_.setCenter(spawnPosition_.x, spawnPosition_.y - ViewYOffset);

	// TEST STUFF
	// Initialize input triggers
	std::vector<std::vector<unsigned int>> inputs =
	{
		{ 2, 3, 6 },					// qcf
		{ 6, 2, 3 },					// dp
		{ 2, 1, 4 },					// qcb
		{ 4, 2, 1 },					// reverse dp
		{ 4, 1, 2, 3, 6},				// hcf
		{ 6, 3, 2, 1, 4},				// hcb
		{ 2, 5, 2 },					// down down
		{ 2, 3, 6, 2, 3, 6 },			// double qcf
		{ 2, 8 },						// flash kick (charge)
		{ 4, 6 },						// sonic boom (charge)
		{ 4, 6, 4, 6 },					// back-charge super
		{ 1, 3, 1, 7 },					// delta super (charge)
	};

	std::vector<unsigned int> buffers =
	{
		constants::INPUT_BUFFER_236,
		constants::INPUT_BUFFER_623,
		constants::INPUT_BUFFER_214,
		constants::INPUT_BUFFER_421,
		constants::INPUT_BUFFER_HCF,
		constants::INPUT_BUFFER_HCB,
		constants::INPUT_BUFFER_22,
		40,
		5,
		10,
		20,
		20,
	};

	for (int i = 0; i < inputs.size(); i++)
	{
		triggerArray_.push_back(std::make_unique<InputTrigger>());
		triggerArray_[i]->setMotion(inputs[i]);
		triggerArray_[i]->setBuffer(buffers[i]);
	}

	triggerArray_[8]->setCharge(40, std::vector<bool>{true, true});	 // flash kick
	triggerArray_[9]->setCharge(40, std::vector<bool>{true, false}); // sonic boom
	triggerArray_[10]->setCharge(40, std::vector<bool>{true, true, true, true}); // double-charge super
	triggerArray_[11]->setCharge(40, std::vector<bool>{false, false, false, true}); // delta super
}

World::~World()
{
}

void World::loadTextures()
{
	textures_.load(Textures::ID::Enkidu,		"media/texture/enkidu/Enkidu.png");
	textures_.load(Textures::ID::Yuzuriha,		"media/texture/yuzuriha/Yuzuriha_idle.png");
	textures_.load(Textures::ID::StageMomiji,	"media/texture/_stage/MomijiShrineScaledx3.png");
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i) // iterate through layer node pointers
	{
		SceneNode::Ptr layer(new SceneNode()); // declare new SceneNode for current element
		sceneLayers_[i] = layer.get(); // std::unique_ptr::get() returns pointer to stored object, no ownership transferred to array

		sceneGraph_.attachChild(std::move(layer)); // attach the newest node to scene graph's root node
	}

	sf::Texture& texture = textures_.get(Textures::ID::StageMomiji);
	sf::IntRect textureRect(worldBounds_);

	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(worldBounds_.left, worldBounds_.top);
	sceneLayers_[Background]->attachChild(std::move(backgroundSprite));

	//std::unique_ptr<Character> aokoTest(new Character(Character::Yuzuriha, textures_));
	//p1Char_ = aokoTest.get();
	//p1Char_->setScale(sf::Vector2f(3.7f, 3.7f));
	//p1Char_->setPosition(spawnPosition_.x, spawnPosition_.y + 50);
	//sceneLayers_[Characters]->attachChild(std::move(aokoTest));

	//std::unique_ptr<Character> enkidu(new Character(Character::Enkidu, textures_));
	p1Char_ = std::make_shared<Character>(Character::Enkidu, textures_);
	p1Char_->setPosition(spawnPosition_.x, spawnPosition_.y - 25);
	sceneLayers_[Player1]->attachChild(p1Char_);

	//std::unique_ptr<Character> yuzuriha(new Character(Character::Yuzuriha, textures_));
	//std::shared_ptr<Character> yuzuriha = std::make_shared<Character>(Character::Yuzuriha, textures_);
	p2Char_ = std::make_shared<Character>(Character::Yuzuriha, textures_);
	p2Char_->setPosition(spawnPosition_.x + 500, spawnPosition_.y - 64);
	p2Char_->flipFacing();
	sceneLayers_[Player2]->attachChild(p2Char_);

	charArray_ = { p1Char_, p2Char_ };

	//PlayerContext P1Context(p1Char_, p1RawInput_, p1NumpadInput_);
	//PlayerContext P2Context(p2Char_, p2RawInput_, p2NumpadInput_);
	//BattleContext BattleContext(&P1Context, &P2Context, &timer_);

	//std::unique_ptr<Character> shun(new Character(Character::Shun, textures_));
	//p1Char_ = shun.get();
	//p1Char_->setScale(sf::Vector2f(5.0f, 5.0f));
	//p1Char_->setPosition(spawnPosition_.x, spawnPosition_.y);
	//sceneLayers_[Characters]->attachChild(std::move(shun));
}

void World::draw()
{
	window_.setView(worldView_);
	window_.draw(sceneGraph_);
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
	"double quarter-circle forward",
	"down-charge to up",
	"back-charge to forward",
	"back-charge super",
	"delta super",
};

void World::update()
{
	// Resolve entity interactions (hitbox/hurtbox overlaps etc.)
	
	// Entity updates (states, action initiation/continuation, controllable entities read in player inputs)

	// "Adapt" functions (collision, facing, etc.)

	// Read in accumulated player input for current update, translate to numpad notation, and add to input buffer
	TaggedInput p1RawInput_ = p1_.getInput();
	TaggedInput p1NumpadInput_ = translateToNumpadInput(p1_.getInput());

	if ((p1NumpadInput_.second & 15) != debugPrevInput_)
	{
		RN_DEBUG("Player {} : Numpad Input -- {}", p1NumpadInput_.first, p1NumpadInput_.second & 15);
		debugPrevInput_ = p1NumpadInput_.second & 15;
	}

	// Testing triggers
	for (int i = 0; i < triggerArray_.size(); i++)
	{
		triggerArray_[i]->update(p1NumpadInput_.second);
		if (triggerArray_[i]->isTriggered())
		{
			RN_DEBUG("Motion input detected -- {}", inputString[i]);
		}
	}

	// Check hitbox/hurtbox overlaps

	// Check if player characters are actionable

		// If actionable, initiate action based on input buffer readout

	if ((p1NumpadInput_.second & 15) == 6)
	{
		p1Char_->walkForward(5.f);
	}
	if ((p1NumpadInput_.second & 15) == 4)
	{
		p1Char_->walkBackward(5.f);
	}
	
	// Forward commands to scene graph
	//while (!commandQueue_.isEmpty())
	//{
	//	sceneGraph_.onCommand(commandQueue_.pop());
	//}

	sceneGraph_.update();
	adaptPlayerPosition();
	adaptPlayerFacing();
	// Get center between players
	float CenterX = std::min(p1Char_->getPosition().x, p2Char_->getPosition().x) + abs((p1Char_->getPosition().x - p2Char_->getPosition().x) / 2);
	worldView_.setCenter(CenterX, p1Char_->getPosition().y - ViewYOffset);

	// RN_DEBUG("Current character coordinates are: ({0}, {1}).", 
	// 	p1Char_->getPosition().x, p1Char_->getPosition().y);
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = p1Char_->getPosition();
	position.x = std::max(position.x, worldBounds_.left + borderDistance);
	position.x = std::min(position.x, worldBounds_.left + worldBounds_.width - borderDistance);
	p1Char_->setPosition(position);
}

void World::adaptPlayerFacing()
{
	for (int i = 0; i < 2; i++) // Iterate through characters
	{
		float xPosition = charArray_[i]->getPosition().x; // Only x-axis matters
		float xPositionOpp = charArray_[abs(1 - i)]->getPosition().x; // Get opponent x-axis position

		// In UNI at least, if you jump over an opponent they change facing as you cross over them. 
		// If hit by an attack after the cross-over, the hit animation reflects your facing and any attack that does not change
		// your facing will flip the opponent's facing BACK to the way they were originally facing.

		if ((charArray_[i]->getPosture() != Character::Posture::Airborne) & (charArray_[i]->isActionable()))
		{
			if (xPosition <= xPositionOpp) // if other character is to the right (REMOVE THE = AFTER COLLISION ADAPT IS FINISHED)
			{
				charArray_[i]->setFacing(Character::Facing::Right);
			}
			else if (xPosition > xPositionOpp) // if other character is to the left
			{
				charArray_[i]->setFacing(Character::Facing::Left);
			}
		}
	}
}

World::TaggedInput World::translateToNumpadInput(const World::TaggedInput& playerRawInput)
{
	// Change bit flag inputs from Player to numpad notation. Keep bit flags for buttons (A = 1 << 4 = 16 etc.). Since numpad 
	// notation doesn't go past 9, the entire numpad + buttons input can be stored as a single int.

	unsigned int numpad = 5; // Neutral

	// If x-axis input matches current character facing
	if ((playerRawInput.second & (Input::Left | Input::Right)) == p1Char_->getFacing())
	{
		numpad += 1;
	}
	// If x-axis input is opposite of current character facing
	if ((playerRawInput.second & (Input::Left | Input::Right)) == (p1Char_->getFacing() ^ Input::Left ^ Input::Right)) // Flip bits on character facing to get opposite
	{
		numpad -= 1;
	}
	// If upward input
	if (playerRawInput.second & Input::Up)
	{
		numpad += 3;
	}
	// If downward input
	if (playerRawInput.second & Input::Down)
	{
		numpad -= 3;
	}

	// Convert the first four bits in playerRawInput.second.second to 0s; preserve bits pertaining to buttons (fifth onward)
	return { playerRawInput.first, numpad + (playerRawInput.second >> 4 << 4) };
}

// TODO: need a function to update character facings

//void World::updateInputBuffer(Player::TaggedInput numpadInput, std::pair<Player::ID, std::deque<unsigned int>> &inputBuffer)
//{
//	inputBuffer.second.push_back(numpadInput.second);
//	while (inputBuffer.second.size() > constants::INPUT_MAX_INPUT_BUFFER)
//	{
//		inputBuffer.second.pop_front();
//	}
//}