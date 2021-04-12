#pragma once

#include <SceneNode.hpp>

class Entity : public SceneNode
{
public:
	void					setVelocity(sf::Vector2f velocity);
	void					setVelocity(float vx, float vy);
	sf::Vector2f			getVelocity() const;
	void					accelerate(sf::Vector2f velocity);

private:
	virtual void			updateCurrent(sf::Time dt);

private:
	sf::Vector2f			mVelocity;
};