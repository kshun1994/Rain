#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"

#include <SFML/Graphics/Sprite.hpp>

class Character : public Entity
{
public:
	enum Type
	{
		Yuzuriha,
		Enkidu,
		Shun,
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