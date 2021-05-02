#pragma once

#include "SceneNode.h"

class Entity : public SceneNode
{
public:
	/*explicit				Entity(int health);
	void					damage(int points);
	void					destroy();

	int						getHitpoints() const;
	bool					isDestroyed() const;*/

	void					setVelocity(sf::Vector2f velocity);
	void					setVelocity(float vx, float vy);
	sf::Vector2f			getVelocity() const;
	void					accelerate(sf::Vector2f velocity);

private:
	virtual void			updateCurrent();

private:
	int						mHealth;
	sf::Vector2f			mVelocity;
};