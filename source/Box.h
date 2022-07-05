#pragma once
#include "SceneNode.h"

// To be inherited by stuff like hitboxes/hurtboxes/collision boxes.

class Box : public SceneNode
{
public:
	enum Type
	{
		None		= 0,
		Push		= 1, // >> 0, // 1
		Hit			= 2, // >> 1, // 2
		Hurt		= 3, // >> 2, // 4
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
							Box(Type type, float xOffset, float yOffset, float width, float height);
							Box(std::string name, Type type, float xOffset, float yOffset, float width, float height);
	virtual					~Box() {};

	virtual void			updateCurrent();

	virtual void			setName(std::string name);
	virtual std::string		getName();

	virtual unsigned int	getCategory() const;

	virtual void			setCollideDims(const float& width, const float& height);
	virtual void			setCollideDims(const sf::Vector2f& dims);
	virtual sf::Vector2f	getCollideDims() const;

	virtual void			setCollideOffset(const float& xOffset, const float& yOffset);
	virtual void			setCollideOffset(const sf::Vector2f& offset);
	virtual sf::Vector2f	getCollideOffset() const;

	virtual void			setType(const unsigned int& type);
	virtual unsigned int	getType() const;

	virtual sf::FloatRect	getRect() const;

	virtual void			moveParent(const float& x, const float& y);

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	float					xOffset_;
	float					xOffsetCorrected_;
	float					yOffset_;
	float					width_;
	float					height_;

	unsigned int			type_;

	std::string				name_;
};
