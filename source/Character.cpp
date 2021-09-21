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
, charStates_(20)
, charStateID_(0)
{
	sprite_.setTexture(textures.get(toTextureID(type)));

	std::vector<int> frameIDs;
	std::vector<int> durations;

	SpriteStruct EnkSprite;
	EnkSprite.spriteDims = sf::Vector2(1600, 768);

	EnkSprite.idleIDs.resize(16);
	std::iota(EnkSprite.idleIDs.begin(), EnkSprite.idleIDs.end(), 0);
	EnkSprite.idleDurs.resize(16);
	std::fill(EnkSprite.idleDurs.begin(), EnkSprite.idleDurs.end(), 5);
		
	EnkSprite.crouchIDs.resize(15);
	std::iota(EnkSprite.crouchIDs.begin(), EnkSprite.crouchIDs.end(), 16);
	EnkSprite.crouchDurs.resize(15);
	std::fill(EnkSprite.crouchDurs.begin(), EnkSprite.crouchDurs.end(), 5);

	// TODO Both forward and back walks have transition-into animations; include those somehow into CharState::enter()
		
	EnkSprite.fWalkIDs.resize(9);
	std::iota(EnkSprite.fWalkIDs.begin(), EnkSprite.fWalkIDs.end(), 31);
	EnkSprite.fWalkDurs.resize(9);
	std::fill(EnkSprite.fWalkDurs.begin(), EnkSprite.fWalkDurs.end(), 5);

	EnkSprite.bWalkIDs.resize(9);
	std::iota(EnkSprite.bWalkIDs.begin(), EnkSprite.bWalkIDs.end(), 42);
	EnkSprite.bWalkDurs.resize(9);
	std::fill(EnkSprite.bWalkDurs.begin(), EnkSprite.bWalkDurs.end(), 5);

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
		std::unique_ptr<StandState> standState = std::make_unique<StandState>();
		standState->setAnimationFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
		standState->setAnimationRepeat(true);

		standState->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		standState->appendBox(std::move(std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 310.f)));

		// Crouching state
		std::unique_ptr<CrouchState> crouchState = std::make_unique<CrouchState>();
		crouchState->setAnimationFrames(spriteStruct_.crouchIDs, spriteStruct_.crouchDurs, spriteStruct_.spriteDims);
		crouchState->setAnimationRepeat(true);

		crouchState->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 180.f, 220.f)));
		crouchState->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, -150.f, 110.f, 100.f)));
		crouchState->appendBox(std::move(std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 200.f)));

		// Forward walk state
		std::unique_ptr<FWalkState> fWalkState = std::make_unique<FWalkState>();
		fWalkState->setAnimationFrames(spriteStruct_.fWalkIDs, spriteStruct_.fWalkDurs, spriteStruct_.spriteDims);
		fWalkState->setAnimationRepeat(true);
		fWalkState->setSpeed(7.f);

		fWalkState->appendBox(std::move(std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f)));
		fWalkState->appendBox(std::move(std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 310.f)));

		// Backward walk state
		std::unique_ptr<BWalkState> bWalkState = std::make_unique<BWalkState>();
		bWalkState->setAnimationFrames(spriteStruct_.bWalkIDs, spriteStruct_.bWalkDurs, spriteStruct_.spriteDims);
		bWalkState->setAnimationRepeat(true);
		bWalkState->setSpeed(5.f);

		std::shared_ptr<Box> hurtBox = std::make_shared<Box>(Box::Type::Hurt, 0.f, 0.f, 140.f, 330.f);
		bWalkState->appendBox(std::move(hurtBox));
		std::shared_ptr<Box> collideBox = std::make_shared<Box>(Box::Type::Collide, 0.f, 0.f, 100.f, 310.f);
		bWalkState->appendBox(std::move(collideBox));

		charStates_[COMMON_ACTION_STAND]	= std::move(standState);
		charStates_[COMMON_ACTION_CROUCH]	= std::move(crouchState);
		charStates_[COMMON_ACTION_F_WALK]	= std::move(fWalkState);
		charStates_[COMMON_ACTION_B_WALK]	= std::move(bWalkState);

		charState_ = charStates_[COMMON_ACTION_STAND].get();		// Start standing
		charState_->enter(*this);
		setCurrentCharStateID(COMMON_ACTION_STAND);

		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_STAND,  false));
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_CROUCH, false));
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_F_WALK, false));
		stateMap_.insert(std::pair<int, bool>(COMMON_ACTION_B_WALK, false));
	}
	else if (type_ == Type::Yuzuriha)
	{
		spriteStruct_ = YuzuSprite;
	}

	sprite_.setFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
	sprite_.setOrigin(spriteStruct_.originX, spriteStruct_.spriteDims.y - spriteStruct_.originY);
	sprite_.setRepeating(true);

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
	charState_->update(*this);
	sprite_.update();
}

unsigned int Character::getCategory() const
{
	return Category::Character;
}

void Character::handleInput(Player::TaggedInput input)
{
	for (int i = 0; i < inputTriggers_.size(); i++)
	{
		inputTriggers_[i]->update(input.second);
	}

	parseInput(input.second);

	int charStateID = charState_->handleInput(*this, stateMap_);

	clearStateMap();

	if (charStateID != NULL_ACTION)
	{
		charState_ = charStates_[charStateID].get();

		charState_->enter(*this);
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

int Character::getCurrentCharStateID()
{
	return charStateID_;
}

void Character::setCurrentCharStateID(int id)
{
	charStateID_ = id;
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
