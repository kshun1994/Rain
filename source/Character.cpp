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
, sprite_(textures.get(toTextureID(type)))
, health_(0.f)
, meter_(0.f)
, facing_(Facing::Right)
, posture_(Posture::Standing)
, actionState_(ActionState::None)
, animationState_(AnimationState::Idle)
, prevAnimationState_(AnimationState::Idle)
, facingSignFlip_(1)
, spriteStruct_()
, charStates_(20)
{
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

	// Both forward and back walks have transition-into animations; include those somehow into CharState::enter()
		
	EnkSprite.fWalkIDs.resize(9);
	std::iota(EnkSprite.fWalkIDs.begin(), EnkSprite.fWalkIDs.end(), 31);
	EnkSprite.fWalkDurs.resize(9);
	std::fill(EnkSprite.fWalkDurs.begin(), EnkSprite.fWalkDurs.end(), 5);

	EnkSprite.bWalkIDs.resize(9);
	std::iota(EnkSprite.bWalkIDs.begin(), EnkSprite.bWalkIDs.end(), 42);
	EnkSprite.bWalkDurs.resize(9);
	std::fill(EnkSprite.bWalkDurs.begin(), EnkSprite.bWalkDurs.end(), 5);

	EnkSprite.originX = 655;

	//for (int i = 0; i != 16; i++)
	//{
	//	EnkSprite.idleIDs.push_back(i);
	//	EnkSprite.idleDurs.push_back(5);
	//}
	//for (int i = 16; i != 25; i++)
	//{
	//	EnkSprite.walkFIDs.push_back(i);
	//	EnkSprite.walkFDurs.push_back(5);
	//}
	//for (int i = 25; i != 36; i++)
	//{
	//	EnkSprite.walkBIDs.push_back(i);
	//	EnkSprite.walkBDurs.push_back(5);
	//}

	SpriteStruct YuzuSprite;
	YuzuSprite.spriteDims = sf::Vector2(864, 640);
	YuzuSprite.originX = 432;
	for (int i = 0; i != 36; i++)
	{
		YuzuSprite.idleIDs.push_back(i);
		YuzuSprite.idleDurs.push_back(5);
	}
	//for (int i = 16; i != 25; i++)
	//{
	//	YuzuSprite.walkFIDs.push_back(i);
	//	YuzuSprite.walkFDurs.push_back(5);
	//}
	//for (int i = 25; i != 36; i++)
	//{
	//	YuzuSprite.walkBIDs.push_back(i);
	//	YuzuSprite.walkBDurs.push_back(5);
	//}


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
			inputTriggers_.push_back(std::make_unique<InputTrigger>());
			inputTriggers_[i]->setMotion(inputs[i]);
			inputTriggers_[i]->setBuffer(buffers[i]);
		}

		inputTriggers_[8]->setCharge(40, std::vector<bool>{true, true});					// flash kick
		inputTriggers_[9]->setCharge(40, std::vector<bool>{true, false});					// sonic boom
		inputTriggers_[10]->setCharge(40, std::vector<bool>{true, true, true, true});		// double-charge super
		inputTriggers_[11]->setCharge(40, std::vector<bool>{false, false, false, true});	// delta super

		spriteStruct_ = EnkSprite;

		std::shared_ptr<StandState> standState = std::make_shared<StandState>();
		standState->setAnimationFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
		standState->setAnimationRepeat(true);

		std::shared_ptr<CrouchState> crouchState = std::make_shared<CrouchState>();
		crouchState->setAnimationFrames(spriteStruct_.crouchIDs, spriteStruct_.crouchDurs, spriteStruct_.spriteDims);
		crouchState->setAnimationRepeat(true);

		std::shared_ptr<FWalkState> fWalkState = std::make_shared<FWalkState>();
		fWalkState->setAnimationFrames(spriteStruct_.fWalkIDs, spriteStruct_.fWalkDurs, spriteStruct_.spriteDims);
		fWalkState->setAnimationRepeat(true);
		fWalkState->setSpeed(10.f);

		std::shared_ptr<BWalkState> bWalkState = std::make_shared<BWalkState>();
		bWalkState->setAnimationFrames(spriteStruct_.bWalkIDs, spriteStruct_.bWalkDurs, spriteStruct_.spriteDims);
		bWalkState->setAnimationRepeat(true);
		bWalkState->setSpeed(7.f);

		charStates_[COMMON_ACTION_STAND]	= standState;
		charStates_[COMMON_ACTION_CROUCH]	= crouchState;
		charStates_[COMMON_ACTION_F_WALK]	= fWalkState;
		charStates_[COMMON_ACTION_B_WALK]	= bWalkState;

		charState_ = charStates_[COMMON_ACTION_STAND];		// Start standing

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
	sprite_.setOrigin(spriteStruct_.originX, spriteStruct_.spriteDims.y);
	sprite_.setRepeating(true);

	health_			= 1000.f;
	meter_			= 0.f;
}

Character::~Character()
{
	//for (CharState* charState : charStates_)
	//{
	//		delete charState;
	//}

	//for (InputTrigger* inputTrigger : inputTriggers_)
	//{
	//	delete inputTrigger;
	//}
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}

void Character::updateCurrent()
{
	charState_->update(*this);
	sprite_.update();
	animationState_ = AnimationState::Idle;
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

	std::shared_ptr<CharState> charState = charState_->handleInput(*this, stateMap_);

	clearStateMap();

	if (charState != nullptr)
	{
		charState_ = charState;

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

float Character::getHealth() const
{
	return health_;
}

float Character::getMeter() const
{
	return meter_;
}

Character::Facing Character::getFacing() const
{
	return facing_;
}

Character::Posture Character::getPosture() const
{
	return posture_;
}

Character::ActionState Character::getActionState() const
{
	return actionState_;
}

bool Character::isActionable() const
{
	return !actionState_;
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

void Character::setMeter(float value)
{
	meter_ = value;
}

void Character::addMeter(float value)
{
	meter_ += value;
	if (meter_ > constants::METER_MAX)
	{
		meter_ = constants::METER_MAX;
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

void Character::setFacing(Facing facing)
{
	if (facing_ != facing)
	{
		facing_ = facing;
		sprite_.scale(-1, 1); // Flip sprite horizontally
	}
	setSignFlip();
}

void Character::flipFacing()
{
	facing_ != facing_;
	sprite_.scale(-1, 1);
	setSignFlip();
}

void Character::setSignFlip()
{
	// If Character faces Right, all forward movement vectors etc. should be positive and backward should be negative
	if (facing_ == Facing::Right)
	{
		facingSignFlip_ = 1;
	}
	else if (facing_ == Facing::Left)
	{
		facingSignFlip_ = -1;
	}
}

void Character::setPosture(Posture posture)
{
	posture_ = posture;
}

void Character::setActionState(ActionState actionState)
{
	actionState_ = actionState;
}

void Character::walkForward(float speed)
{
	animationState_ = AnimationState::WalkF;

	// Walk in the direction the Character is Facing
	this->move(speed * facingSignFlip_, 0.f);
	
	// Change the animation frames for as long as the Player holds forward 
}

void Character::walkBackward(float speed)
{
	animationState_ = AnimationState::WalkB;

	this->move(-speed * facingSignFlip_, 0.f);
}

void Character::setAnimationFrames(const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& rect)
{
	sprite_.setFrames(frameIDs, durations, rect);
}

void Character::setAnimationRepeat(bool flag)
{
	sprite_.setRepeating(flag);
}

std::vector<std::shared_ptr<CharState>> Character::getCharStates()
{
	return charStates_;
}

std::shared_ptr<CharState> Character::getCurrentCharState()
{
	return charState_;
}
