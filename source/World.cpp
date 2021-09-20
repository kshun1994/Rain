#include "rnpch.h"
#include "World.h"

#include "Input.h"


const int StageWidth = 2400;
const int StageHeight = 1008;

World::World(sf::RenderWindow& window, Player& P1, Player& P2)
: window_(window)
, worldView_(window.getDefaultView())
, textures_()
, sceneGraph_()
, sceneLayers_()
, worldBounds_(0.f, 0.f, StageWidth, StageHeight)
, spawnPosition_(0, 0)
, p1_(P1)
, p2_(P2)
, timer_()
, debugPrevInput_(0)
{
	// Need some step where input devices assigned in preceding Device Select State are assigned to players

	loadTextures();
	buildScene();

	// Prepare the view
	worldView_.setCenter(0, 0);
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
	backgroundSprite->setOrigin(StageWidth / 2, StageHeight - 100); // Stage origin should be horizontal center and "ground" that characters stand on
	backgroundSprite->setPosition(0, 0); // Stage has to 
	sceneLayers_[Background]->attachChild(std::move(backgroundSprite));

	p1Char_ = std::make_shared<Character>(Character::Enkidu, textures_);
	p1Char_->setPosition(-250, 0);
	p1Char_->setFacing(Character::Facing::Right);
	sceneLayers_[Characters]->attachChild(p1Char_);

	p2Char_ = std::make_shared<Character>(Character::Enkidu, textures_);
	p2Char_->setPosition(250, 0);
	p2Char_->setFacing(Character::Facing::Left);
	sceneLayers_[Characters]->attachChild(p2Char_);
	
	charArray_ = { p1Char_, p2Char_ };
}

void World::draw()
{
	window_.setView(worldView_);
	window_.draw(sceneGraph_);
}

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


	// Check hitbox/hurtbox overlaps

	// Check if player characters are actionable

		// If actionable, initiate action based on input buffer readout

	charArray_[0]->handleInput(p1NumpadInput_);

	sceneGraph_.update();
	adaptCharacterPosition();
	adaptCharacterFacing();
	adaptCharacterCollision();
	// Get center between players
	float CenterX = std::min(charArray_[0]->getPosition().x, charArray_[1]->getPosition().x) + abs((charArray_[0]->getPosition().x - charArray_[1]->getPosition().x) / 2);
	worldView_.setCenter(CenterX, charArray_[0]->getPosition().y - constants::VIEW_Y_OFFSET);
}

void World::adaptCharacterPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
	const float borderDistance = 200.f;

	sf::Vector2f position = charArray_[0]->getPosition();
	position.x = std::max(position.x, -(StageWidth / 2) + borderDistance);
	position.x = std::min(position.x, (StageWidth / 2) - borderDistance);
	charArray_[0]->setPosition(position);
}

void World::adaptCharacterFacing()
{
	//for (SceneNode* character : sceneLayers_[Characters]->getChildren()) // Iterate through characters

	//std::vector<Character*> characters(sceneLayers_[Characters]->getChildren().size());

	//for (Character* character : sceneLayers_[Characters]->getChildren())
	//{
	//	characters.push_back(static_cast<Character*>(character));
	//}

	for (size_t i = 0; i < charArray_.size(); ++i) // Iterate through characters
	{
		float xPosition = charArray_[i]->getPosition().x; // Only x-axis matters
		float xPositionOpp = charArray_[1 - i]->getPosition().x; // Get opponent x-axis position

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

void World::adaptCharacterCollision()
{
	//// If a character collide box is intersecting with another collide box, shunt the character in the direction of the center of their own collide box
	//for (size_t i = 0; i != 1; ++i)
	//{
	//	// Check if current character collide box intersects with other character's
	//	if (charArray_[i]->checkBoxIntersect(charArray_[1 - i]->getBox().getGlobalBounds()))
	//	{
	//		sf::FloatRect charBox = charArray_[i]->getBox().getGlobalBounds();
	//		sf::FloatRect oppBox  = charArray_[1 - i]->getBox().getGlobalBounds();

	//		// Figure out whether we're to the left or right of the relevant box
	//		if (charArray_[i]->getBox().getOrigin().x <= charArray_[1 - i]->getBox().getOrigin().x) // To left
	//		{
	//			// "Push" opponent character
	//			charArray_[1 - i]->move(sf::Vector2f(3.5f, 0.f));
	//			// Set character position leftward an amount equal to the intersection distance
	//			// Character collide box right edge minus opponent collide box left edge
	//			sf::Vector2f newPosition = sf::Vector2f(charArray_[i]->getPosition().x - ((charBox.left + charBox.width) - oppBox.left), charArray_[i]->getPosition().y);
	//			//charArray_[i]->move(sf::Vector2f(-((charBox.left + charBox.width) - oppBox.left), 0));
	//			charArray_[i]->setPosition(newPosition);
	//		}
	//		else if (charArray_[i]->getBox().getOrigin().x > charArray_[1 - i]->getBox().getOrigin().x) // To right
	//		{

	//		}

	//		// Move character so box no longer intersects in direction of collide box center
	//		// I.e. move character so collide box center is half box size away from the edge of the opponent collide box
	//		

	//	}
	//}
}

World::TaggedInput World::translateToNumpadInput(const World::TaggedInput& playerRawInput)
{
	// Change bit flag inputs from Player to numpad notation. Keep bit flags for buttons (A = 1 << 4 = 16 etc.). Since numpad 
	// notation doesn't go past 9, the entire numpad + buttons input can be stored as a single int.

	unsigned int numpad = 5; // Neutral

	// If x-axis input matches current character facing
	if ((playerRawInput.second & (Input::Left | Input::Right)) == charArray_[0]->getFacing())
	{
		numpad += 1;
	}
	// If x-axis input is opposite of current character facing
	if ((playerRawInput.second & (Input::Left | Input::Right)) == (charArray_[0]->getFacing() ^ Input::Left ^ Input::Right)) // Flip bits on character facing to get opposite
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