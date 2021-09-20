#include "rnpch.h"
#include "Box.h"

Box::Box(Box::Type type, float xOffset, float yOffset, float width, float height)
: type_(type)
, xOffset_(xOffset)
, yOffset_(yOffset)
, width_(width)
, height_(height)
{
}

void Box::updateCurrent()
{
}

unsigned int Box::getCategory() const
{
	return Category::Box;
}

void Box::setCollideDims(const float& width, const float& height)
{
	width_ = width;
	height_ = height;
}

void Box::setCollideDims(const sf::Vector2f& dims)
{
	width_ = dims.x;
	height_ = dims.y;
}

sf::Vector2f Box::getCollideDims() const
{
	return sf::Vector2f(width_, height_);
}

void Box::setCollideOffset(const float& xOffset, const float& yOffset)
{
	xOffset_ = xOffset;
	yOffset_ = yOffset;
}

void Box::setCollideOffset(const sf::Vector2f& offset)
{
	xOffset_ = offset.x;
	yOffset_ = offset.y;
}

sf::Vector2f Box::getCollideOffset() const
{
	return sf::Vector2f(width_, height_);
}

void Box::setType(const Box::Type& type)
{
	type_ = type;
}

Box::Type Box::getType() const
{
	return type_;
}

void Box::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	#ifdef RN_DEBUG

		sf::Color fillColor = BoxDrawColors[magic_enum::enum_integer(type_)];
		fillColor.a = 32;

		sf::RectangleShape drawBox;
		drawBox.setSize(sf::Vector2f(width_, height_));
		drawBox.setOrigin(width_ / 2, height_);
		drawBox.setFillColor(fillColor);
		drawBox.setOutlineColor(BoxDrawColors[magic_enum::enum_integer(type_)]);
		drawBox.setOutlineThickness(1.f);
		drawBox.setPosition(this->getWorldPosition().x + xOffset_, this->getWorldPosition().y + yOffset_);

		target.draw(drawBox);

	#endif // RN_DEBUG
}
