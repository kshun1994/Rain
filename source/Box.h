#pragma once
#include "SceneNode.h"

// To be inherited by stuff like hitboxes/hurtboxes/collision boxes.

class Box : public SceneNode
{
public:
	enum class Type
	{
		None		= 0,
		Collide		= 1,
		Hit			= 2,
		Hurt		= 3,
	};

protected:
	std::vector<sf::Color> BoxDrawColors = {
		sf::Color::Black,
		sf::Color::White, 
		sf::Color::Red,
		sf::Color::Green,
	};

public:
							Box() = default;
							Box(Type type_, float xOffset, float yOffset, float width, float height);
	virtual					~Box() {};

	virtual void			updateCurrent();

	virtual unsigned int	getCategory() const;

	virtual void			setCollideDims(const float& width, const float& height);
	virtual void			setCollideDims(const sf::Vector2f& dims);
	virtual sf::Vector2f	getCollideDims() const;

	virtual void			setCollideOffset(const float& xOffset, const float& yOffset);
	virtual void			setCollideOffset(const sf::Vector2f& offset);
	virtual sf::Vector2f	getCollideOffset() const;

	virtual void			setType(const Type& type);
	virtual Type			getType() const;

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	float					xOffset_;
	float					yOffset_;
	float					width_;
	float					height_;

	Type					type_;
};
