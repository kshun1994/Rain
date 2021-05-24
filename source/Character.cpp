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
: mType(type)
, mSprite(textures.get(toTextureID(type)))
, mHealth(0.f)
, mMeter(0.f)
, mFacing(Facing::Right)
, mPosture(Posture::Standing)
, mActionState(ActionState::None)
, mAnimationState(AnimationState::Idle)
, mPrevAnimationState(AnimationState::Idle)
, mFacingSignFlip(1)
, mSpriteStruct()
{
	std::vector<int> frameIDs;
	std::vector<int> durations;

	SpriteStruct EnkSprite;
	EnkSprite.spriteDims = sf::Vector2(1600, 768);

	EnkSprite.idleIDs.resize(16);
	std::iota(EnkSprite.idleIDs.begin(), EnkSprite.idleIDs.end(), 0);
	EnkSprite.idleDurs.resize(16);
	std::fill(EnkSprite.idleDurs.begin(), EnkSprite.idleDurs.end(), 5);
		
	EnkSprite.walkFIDs.resize(9);
	std::iota(EnkSprite.walkFIDs.begin(), EnkSprite.walkFIDs.end(), 16);
	EnkSprite.walkFDurs.resize(9);
	std::fill(EnkSprite.walkFDurs.begin(), EnkSprite.walkFDurs.end(), 5);

	EnkSprite.walkBIDs.resize(11);
	std::iota(EnkSprite.walkBIDs.begin(), EnkSprite.walkBIDs.end(), 25);
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


	if (mType == Type::Enkidu)
	{
		mSpriteStruct = EnkSprite;
	}
	else if (mType == Type::Yuzuriha)
	{
		mSpriteStruct = YuzuSprite;
	}

	mSprite.setFrames(mSpriteStruct.idleIDs, mSpriteStruct.idleDurs, mSpriteStruct.spriteDims);
	mSprite.setOrigin(mSpriteStruct.originX, mSpriteStruct.spriteDims.y);
	mSprite.setRepeating(true);

	mHealth			= 1000.f;
	mMeter			= 0.f;
	//mPosition		= sf::Vector2f(0.f, 0.f);
}

Character::~Character()
{
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Character::updateCurrent()
{
	// 
	if (mAnimationState != mPrevAnimationState)
	{
		if (mAnimationState == AnimationState::Idle)
		{
			RN_DEBUG("Idle");
			mSprite.setFrames(mSpriteStruct.idleIDs, mSpriteStruct.idleDurs, mSpriteStruct.spriteDims);
			mSprite.setRepeating(true);
		}
		else if (mAnimationState == AnimationState::WalkF)
		{
			RN_DEBUG("WalkF");
			mSprite.setFrames(mSpriteStruct.walkFIDs, mSpriteStruct.walkFDurs, mSpriteStruct.spriteDims);
			mSprite.setRepeating(true);
		}
		else if (mAnimationState == AnimationState::WalkB)
		{
			RN_DEBUG("WalkB");
			mSprite.setFrames(mSpriteStruct.walkBIDs, mSpriteStruct.walkBDurs, mSpriteStruct.spriteDims);
			mSprite.setRepeating(true);
		}
	}

	mPrevAnimationState = mAnimationState;

	mSprite.update();
	mAnimationState = AnimationState::Idle;
}

unsigned int Character::getCategory() const
{
	return Category::Character;
}

void Character::takeInput(Player::TaggedInput input)
{
}

float Character::getHealth() const
{
	return mHealth;
}

float Character::getMeter() const
{
	return mMeter;
}

Character::Facing Character::getFacing() const
{
	return mFacing;
}

Character::Posture Character::getPosture() const
{
	return mPosture;
}

Character::ActionState Character::getActionState() const
{
	return mActionState;
}

bool Character::isActionable() const
{
	return !mActionState;
}

void Character::setHealth(float value)
{
	mHealth = value;
}

void Character::subtractHealth(float value)
{
	mHealth -= value;
	if (mHealth < 0.f)
	{
		mHealth = 0.f;
	}
}

void Character::setMeter(float value)
{
	mMeter = value;
}

void Character::addMeter(float value)
{
	mMeter += value;
	if (mMeter > constants::METER_MAX)
	{
		mMeter = constants::METER_MAX;
	}
}

void Character::subtractMeter(float value)
{
	mMeter -= value;
	if (mMeter < 0.f)
	{
		mMeter = 0.f;
	}
}

void Character::setFacing(Facing facing)
{
	if (mFacing != facing)
	{
		mFacing = facing;
		mSprite.scale(-1, 1); // Flip sprite horizontally
	}
	setSignFlip();
}

void Character::flipFacing()
{
	mFacing != mFacing;
	mSprite.scale(-1, 1);
	setSignFlip();
}

void Character::setSignFlip()
{
	// If Character faces Right, all forward movement vectors etc. should be positive and backward should be negative
	if (mFacing == Facing::Right)
	{
		mFacingSignFlip = 1;
	}
	else if (mFacing == Facing::Left)
	{
		mFacingSignFlip = -1;
	}
}

void Character::setPosture(Posture posture)
{
	mPosture = posture;
}

void Character::setActionState(ActionState actionState)
{
	mActionState = actionState;
}

void Character::walkForward(float speed)
{
	mAnimationState = AnimationState::WalkF;

	// Walk in the direction the Character is Facing
	this->move(speed * mFacingSignFlip, 0.f);
	
	// Change the animation frames for as long as the Player holds forward 
}

void Character::walkBackward(float speed)
{
	mAnimationState = AnimationState::WalkB;

	this->move(-speed * mFacingSignFlip, 0.f);
}
