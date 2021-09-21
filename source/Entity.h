#pragma once

#include "SceneNode.h"
#include "Input.h"

class Entity : public SceneNode
{
public:
	enum Facing
	{
		Left	= Input::Left,
		Right	= Input::Right,
	};

public:
	/*explicit				Entity(int health);
	void					damage(int points);
	void					destroy();

	int						getHitpoints() const;
	bool					isDestroyed() const;*/
							Entity() = default;
							~Entity();

	void					setVelocity(sf::Vector2f velocity);
	void					setVelocity(float vx, float vy);
	sf::Vector2f			getVelocity() const;
	void					accelerate(sf::Vector2f velocity);

	void					setFacing(const Facing& facing);
	Facing					getFacing() const;
	void					flipFacing();
	void					setSignFlip();

private:
	virtual void			updateCurrent();

private:
	int						health_;
	sf::Vector2f			velocity_;

	Facing					facing_;
	float					facingSign_;
};