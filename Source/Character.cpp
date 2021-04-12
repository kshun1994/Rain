#include <Character.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

//Textures::ID toTextureID(Character::Type type)
//{
//	switch (type)
//	{
//	case Character::Yuzuriha:
//		return Textures::Yuzuriha;
//	case Character::Enkidu:
//		return Textures::Enkidu;
//	}
//}

Character::Character(Type type, const TextureHolder& textures): mType(type), mSprite(textures.get(toTextureID(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height); // set origin to horizontal midpoint at bottom of sprite
}

void Character::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}