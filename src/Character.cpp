#include "pch.h"
#include "Character.h"
#include "ResourceHolder.h"
#include "Utility.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


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

//Character::Character(Type type, const TextureHolder& textures)
//	: mType(type)
//	, mSprite(textures.get(toTextureID(type)))
//{
//	sf::FloatRect bounds = mSprite.getLocalBounds();
//	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
//}

Character::Character(Type type, const TextureHolder& textures)
: mType(type)
, mSprite(textures.get(toTextureID(type)))
, mIdleAnimation(textures.get(toTextureID(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height); // set origin to horizontal midpoint at bottom of sprite

	std::vector<int> frameIDs;
	std::vector<int> durations;
	const sf::Vector2i EnkSpriteDims(1600, 768);

	for (int i = 0; i != 16; i++)
	{
		frameIDs.push_back(i);
		durations.push_back(7);
	}
	
	mIdleAnimation.setFrames(frameIDs, durations, EnkSpriteDims);
	mIdleAnimation.setRepeating(true);
	mIdleAnimation.setOrigin(EnkSpriteDims.x / 2.f, EnkSpriteDims.y);
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mIdleAnimation, states);
}

void Character::updateCurrent(sf::Time dt)
{
	mIdleAnimation.update(dt);
}

unsigned int Character::getCategory() const
{
	return Category::Character;
}