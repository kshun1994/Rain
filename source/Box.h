#pragma once
#include "SceneNode.h"

// To be inherited by stuff like hitboxes/hurtboxes/collision boxes.

class Box : public SceneNode
{
public:
	enum Type
	{
		None		= 0,
		Collide		= 1,
		Hit			= 2,
		Hurt		= 3,
	};

public: 
	typedef std::shared_ptr<SceneNode> Ptr;

public:
							Box() = default;
							Box(sf::FloatRect rect);
	virtual					~Box() {};

	virtual void			updateCurrent();

	virtual void			setRect(sf::FloatRect rect);
	virtual sf::FloatRect	getRect();

	virtual void			setType(Type type);
	virtual Type			getType();

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type					type_;
	sf::FloatRect			rect_;
};
