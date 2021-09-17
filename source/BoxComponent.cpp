#include "rnpch.h"
#include "BoxComponent.h"
#include "Entity.h"

BoxComponent::BoxComponent(Entity& entity, Type type, float offsetX, float offsetY, float width, float height)
: entity_(entity)
, offsetX_(offsetX)
, offsetY_(offsetY)
{
	box_.setSize(sf::Vector2f(width, height));
	box_.setOrigin(sf::Vector2f(width / 2, height));
	box_.setPosition(entity_.getPosition().x + offsetX_, entity_.getPosition().y + offsetY_);
	box_.setFillColor(sf::Color::Transparent);
	box_.setOutlineThickness(1.f);
	box_.setOutlineColor(sf::Color::White);

	type_ = type;
}

BoxComponent::~BoxComponent()
{
}

void BoxComponent::update()
{
	box_.setPosition(entity_.getPosition().x + offsetX_, entity_.getPosition().y + offsetY_);
}

void BoxComponent::draw(sf::RenderTarget& target)
{
	target.draw(box_);
}

BoxComponent::Type BoxComponent::getType()
{
	return type_;
}

sf::RectangleShape BoxComponent::getBox()
{
	return box_;
}

bool BoxComponent::checkIntersect(const sf::FloatRect& rect)
{
	return box_.getGlobalBounds().intersects(rect);
}
