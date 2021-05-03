#include "rnpch.h"
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

Character::Character(Type type, const TextureHolder& textures)
: mType(type)
, mSprite(textures.get(toTextureID(type)))
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