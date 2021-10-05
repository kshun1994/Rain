#include "rnpch.h"
#include "Character.h"
#include "ResourceHolder.h"
#include "Utility.h"


Textures::ID toTextureID(Character::Type type)
{
	switch (type)
	{
	case Character::Yuzuriha:
		return Textures::ID::Yuzuriha;
	case Character::Enkidu:
		return Textures::ID::Enkidu;
	case Character::Shun:
		return Textures::ID::Shun;
	}
}

Character::Character(Type type, const TextureHolder& textures)
: type_(type)
, health_(0.f)
, meter_(0.f)
, facing_(Facing::Right)
, posture_(Posture::Standing)
, spriteStruct_()
, actions_(20)
, actionID_(0)
{
	sprite_.setTexture(textures.get(toTextureID(type)));

	std::vector<int> frameIDs;
	std::vector<int> durations;

	SpriteStruct EnkSprite;
	EnkSprite.spriteDims = sf::Vector2(1600, 768);

	EnkSprite.idleIDs.resize(16);
	std::iota(EnkSprite.idleIDs.begin(), EnkSprite.idleIDs.end(), 0);
	EnkSprite.idleDurs.resize(16, 5);
		
	EnkSprite.crouchIDs.resize(29); // First 3 frames are stand-to-crouch, last 10 frames are crouch-to-stand
	std::iota(EnkSprite.crouchIDs.begin(), EnkSprite.crouchIDs.end(), 16);
	EnkSprite.crouchDurs.resize(EnkSprite.crouchIDs.size(), 5);

	EnkSprite.fWalkIDs.resize(9);
	std::iota(EnkSprite.fWalkIDs.begin(), EnkSprite.fWalkIDs.end(), 54);
	EnkSprite.fWalkDurs.resize(EnkSprite.fWalkIDs.size(), 5);

	EnkSprite.bWalkIDs.resize(11);
	std::iota(EnkSprite.bWalkIDs.begin(), EnkSprite.bWalkIDs.end(), 63);
	EnkSprite.bWalkDurs.resize(EnkSprite.bWalkIDs.size(), 5);

	std::vector<int> crouchToStandInds = {35, 36, 37, 38, 39, 40, 41, 42, 43, 44};
	EnkSprite.jumpIDs.resize(8); 
	std::iota(EnkSprite.jumpIDs.begin(), EnkSprite.jumpIDs.end(), 45);
	// Jump animation has to include crouch-to-stand as recovery animation
	EnkSprite.jumpIDs.insert(EnkSprite.jumpIDs.end(), std::make_move_iterator(crouchToStandInds.begin()), std::make_move_iterator(crouchToStandInds.end()));
	EnkSprite.jumpDurs.resize(EnkSprite.jumpIDs.size(), 5);
	EnkSprite.jumpDurs[0] = 4; // Jump startup is 4f why not

	EnkSprite.standBIDs.resize(8);
	std::iota(EnkSprite.standBIDs.begin(), EnkSprite.standBIDs.end(), 74);
	EnkSprite.standBDurs.resize(EnkSprite.standBIDs.size(), 5);

	EnkSprite.originX = 635.f; // Horizontal center of sprite (regardless of original image dimensions)
	EnkSprite.originY = 83.f;  // Distance from bottom of image where sprite "touches the ground"


	SpriteStruct YuzuSprite;
	YuzuSprite.spriteDims = sf::Vector2(864, 640);
	YuzuSprite.originX = 432;
	YuzuSprite.originY = 44;
	for (int i = 0; i != 36; i++)
	{
		YuzuSprite.idleIDs.push_back(i);
		YuzuSprite.idleDurs.push_back(5);
	}

	if (type_ == Type::Enkidu)
	{
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
			{ 1, 3, 1, 9 },					// delta super (charge)
		};

		std::vector<unsigned int> buffers =
		{
			Constants::INPUT_BUFFER_236,
			Constants::INPUT_BUFFER_623,
			Constants::INPUT_BUFFER_214,
			Constants::INPUT_BUFFER_421,
			Constants::INPUT_BUFFER_HCF,
			Constants::INPUT_BUFFER_HCB,
			Constants::INPUT_BUFFER_22,
			40,
			5,
			10,
			20,
			20,
		};

		for (int i = 0; i < inputs.size(); i++)
		{
			inputTriggers_.push_back(std::make_unique<InputTrigger>());
			inputTriggers_[i]->setMotion(inputs[i]);
			inputTriggers_[i]->setBuffer(buffers[i]);
		}

		inputTriggers_[8]->setCharge(40, std::vector<bool>{true, true});					// flash kick
		inputTriggers_[9]->setCharge(40, std::vector<bool>{true, false});					// sonic boom
		inputTriggers_[10]->setCharge(40, std::vector<bool>{true, true, true, true});		// double-charge super
		inputTriggers_[11]->setCharge(40, std::vector<bool>{false, false, false, true});	// delta super

		spriteStruct_ = EnkSprite;

		// Create Enkidu standing state
		std::unique_ptr<StandAction> standAction = std::make_unique<StandAction>();
		standAction->setAnimationFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
		standAction->setLoopBounds(0, std::accumulate(spriteStruct_.idleDurs.begin(), spriteStruct_.idleDurs.end(), 0) - 1);
		standAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		standAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 310.f)));

		// Crouching state
		std::unique_ptr<CrouchAction> crouchAction = std::make_unique<CrouchAction>();
		crouchAction->setAnimationFrames(spriteStruct_.crouchIDs, spriteStruct_.crouchDurs, spriteStruct_.spriteDims);
		crouchAction->setLoopBounds(15, 94);
		crouchAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 180.f, 220.f)));
		crouchAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, -150.f, 110.f, 100.f)));
		crouchAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 200.f)));

		// Forward walk state
		std::unique_ptr<FWalkAction> fWalkAction = std::make_unique<FWalkAction>();
		fWalkAction->setAnimationFrames(spriteStruct_.fWalkIDs, spriteStruct_.fWalkDurs, spriteStruct_.spriteDims);
		fWalkAction->setLoopBounds(0, std::accumulate(spriteStruct_.fWalkDurs.begin(), spriteStruct_.fWalkDurs.end(), 0) - 1);
		fWalkAction->setMovePerFrame(std::vector<sf::Vector2f>(std::accumulate(spriteStruct_.fWalkDurs.begin(), spriteStruct_.fWalkDurs.end(), 0), sf::Vector2f(7.f, 0)));
		//std::vector<sf::Vector2f> movePerFrame(std::accumulate(spriteStruct_.fWalkDurs.begin(), spriteStruct_.fWalkDurs.end(), 0));
		//float maxSpeed = 20.f;
		//for (int i = 0; i < movePerFrame.size(); ++i)
		//{
		//	movePerFrame[i].x = (maxSpeed / 2.f) * sinf(((2.f * M_PI) / movePerFrame.size()) * (i - (static_cast<float>(movePerFrame.size()) / 4.f))) + (maxSpeed / 2.f);
		//}
		//fWalkAction->setMovePerFrame(movePerFrame);
		fWalkAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		fWalkAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 310.f)));

		// Backward walk state
		std::unique_ptr<BWalkAction> bWalkAction = std::make_unique<BWalkAction>();
		bWalkAction->setAnimationFrames(spriteStruct_.bWalkIDs, spriteStruct_.bWalkDurs, spriteStruct_.spriteDims);
		bWalkAction->setLoopBounds(10, std::accumulate(spriteStruct_.bWalkDurs.begin(), spriteStruct_.bWalkDurs.end(), 0) - 1);
		bWalkAction->setMovePerFrame(std::vector<sf::Vector2f>(std::accumulate(spriteStruct_.bWalkDurs.begin(), spriteStruct_.bWalkDurs.end(), 0), sf::Vector2f(-7.f, 0)));
		std::shared_ptr<Box> hurtBox = std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f);
		bWalkAction->appendBox(std::move(hurtBox));
		std::shared_ptr<Box> collideBox = std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 310.f);
		bWalkAction->appendBox(std::move(collideBox));

		// Jump forward state
		// TODO Make a Jump subclass for Action that just takes in a peak height, duration, and distance travelled and does all of this automatically
		// Below equations have gravity in units of pixels per game tick ^ 2
		// Time of flight T = (2 * ySpeedInitial) / gravity
		// ySpeed at any time t = ySpeedInitial - (gravity * t)
		// peakHeight = (ySpeedInitial ^ 2) / (2 * gravity)
		// xDistTravel = (((xSpeedInitial ^ 2) + (ySpeedInitial ^ 2)) * sinf(2 * (tanf(ySpeedInitial / xSpeedInitial)))) / gravity
		//
		// Can calculate gravity or ySpeedInitial given the other and total duration
		//     gravity = (2 * ySpeedInitial) / duration
		//     ySpeedInitial = (gravity * duration) / 2
		// Can calculate ySpeedInitial given peakHeight and gravity
		//     ySpeedInitial = sqrt(2 * gravity * peakHeight)
		// Can calculate gravity given ySpeedInitial and peakHeight
		//     gravity = (ySpeedInitial ^ 2) / (2 * peakHeight)
		// Can calculate xSpeed given gravity, ySpeedInitial, and range (total horizontal distance travelled)
		//     xSpeed = (gravity * range) / (2 * ySpeedInitial)
		//
		// Once above has been calculated, the number of frames to display each jump animation frame can be calculated

		std::unique_ptr<JumpAction> fJumpAction = std::make_unique<JumpAction>();
		fJumpAction->setAnimationFrames(spriteStruct_.jumpIDs, spriteStruct_.jumpDurs, spriteStruct_.spriteDims);
		int jumpStartup = 4;
		int jumpDuration = 35;
		std::vector<int> startupInds;
		std::vector<int> activeInds;
		for (int i = 0; i < std::accumulate(spriteStruct_.jumpDurs.begin(), spriteStruct_.jumpDurs.end(), 0); ++i)
		{
			if (i < jumpStartup)
			{
				startupInds.push_back(i);
			}
			if ((jumpStartup <= i) && (i < (jumpStartup + jumpDuration)))
			{
				activeInds.push_back(i);
			}
		}
		fJumpAction->addProperty(Action::Property::Startup, startupInds);
		fJumpAction->addProperty(Action::Property::Active, activeInds);
		fJumpAction->setJumpArcViaInitialY(15.f, 500.f, 500.f, 35);

		// Jump has 1 animation frame of startup, 8 animation frames of travel, 10 animation frames of (cancellable) recovery

		actions_[COMMON_ACTION_STAND]	= std::move(standAction);
		actions_[COMMON_ACTION_CROUCH]	= std::move(crouchAction);
		actions_[COMMON_ACTION_F_WALK]	= std::move(fWalkAction);
		actions_[COMMON_ACTION_B_WALK]	= std::move(bWalkAction);
		actions_[COMMON_ACTION_F_JUMP]	= std::move(fJumpAction);

		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_STAND,  false));
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_CROUCH, false));
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_F_WALK, false));
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_B_WALK, false));
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_F_JUMP, false));
	}
	else if (type_ == Type::Yuzuriha)
	{
		spriteStruct_ = YuzuSprite;

		// Create Yuzu standing state
		std::unique_ptr<StandAction> standAction = std::make_unique<StandAction>();
		standAction->setAnimationFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
		standAction->setLoopBounds(0, std::accumulate(spriteStruct_.idleDurs.begin(), spriteStruct_.idleDurs.end(), 0) - 1);

		standAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 110.f, 315.f)));
		standAction->appendBox(std::move(std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 310.f)));

		actions_[COMMON_ACTION_STAND] = std::move(standAction);
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_STAND,  false));
	}

	action_ = actions_[COMMON_ACTION_STAND].get(); // Start standing
	action_->enter(*this);
	setCurrentActionID(COMMON_ACTION_STAND);

	sprite_.setFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
	sprite_.setOrigin(spriteStruct_.originX, spriteStruct_.spriteDims.y - spriteStruct_.originY);

	health_			= 1000.f;
	meter_			= 0.f;
}

Character::~Character()
{
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);

	#ifdef RN_DEBUG
		// Draw a cross at character's origin position
		float segmentLength = 20.f;
		sf::Vector2f coord = this->getPosition();
		//RN_DEBUG("Current position - ({}, {})", coord.x, coord.y);

		sf::Vertex vertLine[] =
		{
			sf::Vertex(sf::Vector2f(coord.x, coord.y - segmentLength), sf::Color::Cyan),
			sf::Vertex(sf::Vector2f(coord.x, coord.y + segmentLength), sf::Color::Cyan)
		};
		sf::Vertex horiLine[] =
		{
			sf::Vertex(sf::Vector2f(coord.x - segmentLength, coord.y), sf::Color::Cyan),
			sf::Vertex(sf::Vector2f(coord.x + segmentLength, coord.y), sf::Color::Cyan)
		};

		target.draw(vertLine, 2, sf::Lines);
		target.draw(horiLine, 2, sf::Lines);
	#endif // RN_DEBUG
}

void Character::updateCurrent()
{
	action_->update(*this);
	sprite_.update();
}

unsigned int Character::getCategory() const
{
	return Category::Character;
}

Character::Type Character::getType() const
{
	return type_;
}

void Character::handleInput(Player::TaggedInput input)
{
	for (int i = 0; i < inputTriggers_.size(); i++)
	{
		inputTriggers_[i]->update(input.second);
	}

	parseInput(input.second);

	int actionID = action_->handleInput(*this, stateMap_);

	clearStateMap();

	if (actionID != CONTINUE_ACTION)
	{
		action_ = actions_[actionID].get();

		action_->enter(*this);
	}
}

void Character::parseInput(unsigned int input)
{
	// Flip bools in stateMap_ based on current input
	
	// Check for forward walk
	if ((input & 15) == 6)
	{
		stateMap_[COMMON_ACTION_F_WALK] = true;
	}
	
	// Check for forward jump
	if ((input & 15) == 9)
	{
		stateMap_[COMMON_ACTION_F_JUMP] = true;
	}
	
	// Check for back walk
	if ((input & 15) == 4)
	{
		stateMap_[COMMON_ACTION_B_WALK] = true;
	}
	
	// Check for crouch
	if ((input & 15) <= 3)
	{
		stateMap_[COMMON_ACTION_CROUCH] = true;
	}

	// Check for stand (idle)
	if ((input & 15) == 5)
	{
		stateMap_[COMMON_ACTION_STAND] = true;
	}

	std::vector<std::string> inputString =
	{
		"quarter circle forward",
		"dragon punch",
		"quarter circle backward",
		"reverse dragon punch",
		"half circle forward",
		"half circle backward",
		"down down",
		"double quarter circle forward",
		"down-charge to up",
		"back-charge to forward",
		"back-charge super",
		"delta super",
	};

	for (int i = 0; i < inputTriggers_.size(); i++)
	{
		if (inputTriggers_[i]->isTriggered())
		{
			RN_DEBUG("Motion input detected: {}.", inputString[i]);
			//stateMap_[COMMON_ACTION_BACK_CHARGE] = true;
		}
	}
}

void Character::clearStateMap()
{
	// Sets all bools in stateMap to false; to be used after inputs have been parsed for the current frame
	for (std::pair<int, bool> statePair : stateMap_)
	{
		stateMap_[statePair.first] = false;
	}
}

int Character::getCurrentActionID()
{
	return actionID_;
}

void Character::setCurrentActionID(int id)
{
	actionID_ = id;
}

float Character::getHealth() const
{
	return health_;
}

void Character::setHealth(float value)
{
	health_ = value;
}

void Character::subtractHealth(float value)
{
	health_ -= value;
	if (health_ < 0.f)
	{
		health_ = 0.f;
	}
}

float Character::getMeter() const
{
	return meter_;
}

void Character::setMeter(float value)
{
	meter_ = value;
}

void Character::addMeter(float value)
{
	meter_ += value;
	if (meter_ > Constants::METER_MAX)
	{
		meter_ = Constants::METER_MAX;
	}
}

void Character::subtractMeter(float value)
{
	meter_ -= value;
	if (meter_ < 0.f)
	{
		meter_ = 0.f;
	}
}

Character::Posture Character::getPosture() const
{
	return posture_;
}

void Character::setPosture(Posture posture)
{
	posture_ = posture;
}

void Character::setAnimationFrames(const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& rect)
{
	sprite_.setFrames(frameIDs, durations, rect);
}

void Character::setAnimationRepeat(bool flag)
{
	sprite_.setRepeating(flag);
}

void Character::setCurrentAnimationTick(const int& tick)
{
	sprite_.setCurrentTick(tick);
}
