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
		
	EnkSprite.walkFIDs.resize(9);
	std::iota(EnkSprite.walkFIDs.begin(), EnkSprite.walkFIDs.end(), 31);
	EnkSprite.walkFDurs.resize(9);
	std::fill(EnkSprite.walkFDurs.begin(), EnkSprite.walkFDurs.end(), 5);

	EnkSprite.walkBIDs.resize(11);
	std::iota(EnkSprite.walkBIDs.begin(), EnkSprite.walkBIDs.end(), 40);
	EnkSprite.walkBDurs.resize(11);
	std::fill(EnkSprite.walkBDurs.begin(), EnkSprite.walkBDurs.end(), 5);

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
		spriteStruct_ = EnkSprite;

		StandState* standState = new StandState();
		standState->setAnimationFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
		standState->setAnimationRepeat(true);

		CrouchState* crouchState = new CrouchState();
		crouchState->setAnimationFrames(spriteStruct_.crouchIDs, spriteStruct_.crouchDurs, spriteStruct_.spriteDims);
		crouchState->setAnimationRepeat(true);

		charStates_.push_back(standState);
		charStates_.push_back(crouchState);

		charState_ = charStates_[0];
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
	//position_		= sf::Vector2f(0.f, 0.f);
}

Character::~Character()
{
	for (CharState* charState : charStates_)
	{
		delete charState;
	}
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}

void Character::updateCurrent()
{
	// 
	//if (animationState_ != prevAnimationState_)
	//{
	//	if (animationState_ == AnimationState::Idle)
	//	{
	//		RN_DEBUG("Idle");
	//		sprite_.setFrames(spriteStruct_.idleIDs, spriteStruct_.idleDurs, spriteStruct_.spriteDims);
	//		sprite_.setRepeating(true);
	//	}
	//	else if (animationState_ == AnimationState::WalkF)
	//	{
	//		RN_DEBUG("WalkF");
	//		sprite_.setFrames(spriteStruct_.walkFIDs, spriteStruct_.walkFDurs, spriteStruct_.spriteDims);
	//		sprite_.setRepeating(true);
	//	}
	//	else if (animationState_ == AnimationState::WalkB)
	//	{
	//		RN_DEBUG("WalkB");
	//		sprite_.setFrames(spriteStruct_.walkBIDs, spriteStruct_.walkBDurs, spriteStruct_.spriteDims);
	//		sprite_.setRepeating(true);
	//	}
	//}

	//prevAnimationState_ = animationState_;

	sprite_.update();
	animationState_ = AnimationState::Idle;
}

unsigned int Character::getCategory() const
{
	return Category::Character;
}

void Character::handleInput(Player::TaggedInput input)
{
	CharState* charState = charState_->handleInput(*this, input.second);

	if (charState != nullptr)
	{
		charState_ = charState;

		charState_->enter(*this);
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

std::vector<CharState*> Character::getCharStates()
{
	return charStates_;
}
