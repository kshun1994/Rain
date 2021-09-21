#include "rnpch.h"
#include "Entity.h"

Entity::Entity()
: sprite_()
, facing_(Facing::Right)
, facingSign_(1.f)
, health_()
{
}

Entity::~Entity()
{
}

Entity::Facing Entity::getFacing() const
{
	return facing_;
}

void Entity::setFacing(const Facing& facing)
{
	if (facing_ != facing)
	{
		facing_ = facing;
		//sprite_.scale(-1, 1); // Flip sprite horizontally
		flipSprite();
	}
	setSignFlip();
}

void Entity::flipFacing()
{
	facing_ != facing_;
	//sprite_.scale(-1, 1);
	flipSprite();
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

float Entity::getFacingSign() const
{
	return facingSign_;
}

void Entity::flipSprite()
{
	sprite_.scale(-1, 1);
}
