#include "rnpch.h"
#include "Entity.h"

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::updateCurrent()
{
	move(mVelocity);
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}