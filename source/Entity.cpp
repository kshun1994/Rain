#include "rnpch.h"
#include "Entity.h"

Entity::~Entity()
{
	delete this->boxComponent_;
}

void Entity::createBoxComponent(Entity& entity, BoxComponent::Type type, float offsetX, float offsetY, float width, float height)
{
	this->boxComponent_ = new BoxComponent(entity, type, offsetX, offsetY, width, height);
}

bool Entity::checkBoxIntersect(sf::FloatRect rect)
{
	return boxComponent_->checkIntersect(rect);
}

sf::RectangleShape Entity::getBox()
{
	return boxComponent_->getBox();
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