#include "rnpch.h"
#include "Entity.h"

Entity::~Entity()
{
}

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

void Entity::setFacing(const Facing& facing)
{
	if (facing_ != facing)
	{
		facing_ = facing;
	}

	setSignFlip();
}

Entity::Facing Entity::getFacing() const
{
	return facing_;
}

void Entity::flipFacing()
{
	facing_ != facing_;
	setSignFlip();
}

void Entity::setSignFlip()
{
	// If Entity faces Right, all forward movement vectors etc. should be positive and backward should be negative
	if (facing_ == Facing::Right)
	{
		facingSign_ = 1;
	}
	else if (facing_ == Facing::Left)
	{
		facingSign_ = -1;
	}
}
