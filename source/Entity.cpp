#include "rnpch.h"
#include "Entity.h"

void Entity::setVelocity(sf::Vector2f velocity)
{
	velocity_ = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	velocity_.x = vx;
	velocity_.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return velocity_;
}

void Entity::updateCurrent()
{
	move(velocity_);
}

void Entity::accelerate(sf::Vector2f velocity)
{
	velocity_ += velocity;
}