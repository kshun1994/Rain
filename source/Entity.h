#pragma once

#include "SceneNode.h"
#include "Animation.h"
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
							Entity();
							~Entity();


	void					setFacing(const Facing& facing);
	Facing					getFacing() const;
	void					flipFacing();
	void					setSignFlip();
	float					getFacingSign() const;

protected:
	void					flipSprite();

protected:
	Animation				sprite_;

	Facing					facing_;
	float					facingSign_;

private:
	int						health_;
};