#pragma once

#include <Entity.hpp>
#include <ResourceIdentifiers.hpp>
#include <Animation.hpp>
#include <CommandQueue.hpp>

#include <SFML/Graphics/Sprite.hpp>

class Character : public Entity
{
public:
	enum Type
	{
		Yuzuriha,
		Enkidu,
	};

public:
	explicit			Character(Type type, const TextureHolder& textures);
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void		updateCurrent(sf::Time dt);

	unsigned int		getCategory() const;

private:
	Type				mType;
	sf::Sprite			mSprite;
	Animation			mIdleAnimation;
};