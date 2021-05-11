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
, mHealth()
, mMeter()
, mPosition()
, mFacing()
, mPosture()
, mActionState()
{
	std::vector<int> frameIDs;
	std::vector<int> durations;
	const sf::Vector2i EnkSpriteDims(1600, 768);

	for (int i = 0; i != 16; i++)
	{
		frameIDs.push_back(i);
		durations.push_back(7);
	}
	
	mSprite.setFrames(frameIDs, durations, EnkSpriteDims);
	mSprite.setRepeating(true);
	mSprite.setOrigin(EnkSpriteDims.x / 2.f, EnkSpriteDims.y);

	mHealth			= 1000.f;
	mMeter			= 0.f;
	mPosition		= sf::Vector2f(0.f, 0.f);
	mFacing			= Facing::Right;
	mPosture		= Posture::Standing;
	mActionState	= ActionState::None;
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Character::updateCurrent()
{
	mSprite.update();
}

unsigned int Character::getCategory() const
{
	return Category::Character;
}

float Character::getHealth() const
{
	return mHealth;
}

float Character::getMeter() const
{
	return mMeter;
}

sf::Vector2f Character::getPosition() const
{
	return mPosition;
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
	if (mMeter > CONST_METER_MAX)
	{
		mMeter = CONST_METER_MAX;
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

void Character::setPosition(sf::Vector2f position)
{
	mPosition = position;
}

void Character::setPosition(float x, float y)
{
	mPosition = sf::Vector2f(x, y);
}

void Character::setFacing(Facing facing)
{
	mFacing = facing;
}

void Character::setPosture(Posture posture)
{
	mPosture = posture;
}

void Character::setActionState(ActionState actionState)
{
	mActionState = actionState;
}
