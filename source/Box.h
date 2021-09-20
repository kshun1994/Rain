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

	virtual void			setDims(float width, float height);
	virtual void			setDims(sf::Vector2f dims);
	virtual sf::Vector2f	getDims();

	virtual void			setOffset(float xOffset, float yOffset);
	virtual void			setOffset(sf::Vector2f offset);
	virtual sf::Vector2f	getOffset();

	virtual void			setType(Type type);
	virtual Type			getType();

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	float					xOffset_;
	float					yOffset_;
	float					width_;
	float					height_;

	Type					type_;
};
