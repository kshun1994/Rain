#include "rnpch.h"
#include "Box.h"

Box::Box(sf::FloatRect rect)
: type_(Box::Type::None)
, rect_(rect)
{
}

void Box::updateCurrent()
{
}

void Box::setRect(sf::FloatRect rect)
{
	rect_ = rect;
}

sf::FloatRect Box::getRect()
{
	return rect_;
}

void Box::setType(Box::Type type)
{
	type_ = type;
}

Box::Type Box::getType()
{
	return type_;
}

void Box::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	#ifdef RN_DEBUG

	// NEEDS TO BE FIXED
	// Currently moving a character only applies transforms to the drawn rect, not to the Box itself
	// Need rects for drawing to be created based on the Box instead of independently

	// Draw the relevant box
	sf::Vertex rectangle[] =
	{
		sf::Vertex(sf::Vector2f(rect_.left,					rect_.top), sf::Color::White),
		sf::Vertex(sf::Vector2f(rect_.left + rect_.width,	rect_.top), sf::Color::White),
		sf::Vertex(sf::Vector2f(rect_.left + rect_.width,	rect_.top + rect_.height), sf::Color::White),
		sf::Vertex(sf::Vector2f(rect_.left,					rect_.top + rect_.height), sf::Color::White),
		sf::Vertex(sf::Vector2f(rect_.left,					rect_.top), sf::Color::White)
	};

	target.draw(rectangle, 5, sf::LineStrip, states);

	#endif // RN_DEBUG

	RN_DEBUG("Current box position - ({}, {})", this->getPosition().x, this->getPosition().y);
}
