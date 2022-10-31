#include "rnpch.h"
#include "Box.h"
#include "Entity.h"
#include "Character.h"

Box::Box(Box::Type type, float xOffset, float yOffset, float width, float height)
: name_("Default")
, type_(type)
, xOffset_(xOffset)
, xOffsetCorrected_(xOffset)
, yOffset_(yOffset)
, width_(width)
, height_(height)
{
}

Box::Box(std::string name, Type type, float xOffset, float yOffset, float width, float height)
: name_(name)
, type_(type)
, xOffset_(xOffset)
, xOffsetCorrected_(xOffset)
, yOffset_(yOffset)
, width_(width)
, height_(height)
{
}

void Box::updateCurrent()
{
	// Flip offset relative to parent if parent flips facing
	xOffsetCorrected_ = dynamic_cast<Entity*>(parent_)->getFacing() == Entity::Facing::Right ? xOffset_ : -1 * xOffset_;
	//if (xOffset_ >= 0)
	//{
	//}
	//else
	//{
	//	xOffset_ = dynamic_cast<Entity*>(parent_)->getFacing() == Entity::Facing::Right ? -1 * abs(xOffset_) : abs(xOffset_);
	//}
}

void Box::setName(std::string name)
{
	name_ = name;
}

std::string Box::getName()
{
	return name_;
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
	return sf::Vector2f(xOffsetCorrected_, yOffset_);
	RN_DEBUG("Function called!");
}

void Box::setType(const unsigned int& type)
{
	type_ = type;
}

unsigned int Box::getType() const
{
	return type_;
}

void Box::setOnHit(const HitValues& onHit)
{
	onHit_ = onHit;
}

Box::HitValues Box::getOnHit()
{
	return onHit_;
}

void Box::setOnBlock(const HitValues& onBlock)
{
	onBlock_ = onBlock;
}

Box::HitValues Box::getOnBlock()
{
	return onBlock_;
}

void Box::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	#ifdef RN_DEBUG

		sf::Color fillColor = BoxDrawColors[(type_)];
		//fillColor.a = 32;
		fillColor.a = 0;

		sf::RectangleShape drawBox;
		drawBox.setSize(sf::Vector2f(width_, height_));
		drawBox.setOrigin(width_ / 2, height_);
		drawBox.setFillColor(fillColor);
		drawBox.setOutlineColor(BoxDrawColors[type_]);
		drawBox.setOutlineThickness(2.f);
		drawBox.setPosition(this->getWorldPosition().x + xOffsetCorrected_, this->getWorldPosition().y + yOffset_);

		target.draw(drawBox);

	#endif // RN_DEBUG
}

sf::FloatRect Box::getRect() const
{
	return sf::FloatRect(this->getWorldPosition().x + xOffsetCorrected_ - (width_ / 2), 
						 this->getWorldPosition().y + yOffset_ - height_, 
						 width_, height_);
}

void Box::moveParent(const float& x, const float& y)
{
	parent_->move(x, y);
}

void Box::hitParent(const HitValues& hitValues)
{
	int actionID;

	switch (hitValues.impact)
	{
		case Box::Impact::Light:
			actionID = COMMON_ACTION_STAND_HITSTUN_L;
			break;

		case Box::Impact::Heavy:
			actionID = COMMON_ACTION_STAND_HITSTUN_H;
			break;
	}

	dynamic_cast<Character*>(parent_)->setStunDuration(actionID, hitValues.stunDuration);
	dynamic_cast<Character*>(parent_)->overwriteActionBallistics(actionID, hitValues.ballisticLaunchVelocity, hitValues.ballisticLaunchAngle, hitValues.ballisticDerivatives);
	dynamic_cast<Character*>(parent_)->setCurrentActionID(actionID);
}
