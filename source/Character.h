#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"
#include "Action.h"

#include <SFML/Graphics/Sprite.hpp>

class Character : public Entity
{
public:
	enum Type
	{
		Yuzuriha,
		Enkidu,
		Shun,
	};

	enum Facing
	{
		Left	= Action::Left,
		Right	= Action::Right,
	};

	enum Posture
	{
		Standing,
		Crouching,
		Airborne,
	};

	#define Actionable 0
	#define NotActionable 1

	enum ActionState
	{
		None			= Actionable,
		Recovery		= NotActionable + 0,
		Hitstun			= NotActionable + 1,
		Blockstun		= NotActionable + 2,
		Knockdown		= NotActionable + 3,
	};

public:
	explicit			Character(Type type, const TextureHolder& textures);
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void		updateCurrent();

	unsigned int		getCategory() const;

	float				getHealth() const;
	float				getMeter() const;
	sf::Vector2f		getPosition() const;
	Facing				getFacing() const;
	Posture				getPosture() const;
	ActionState			getActionState() const;
	bool				isActionable() const;

	void				setHealth(float value);
	void				subtractHealth(float value);
	void				setMeter(float value);
	void				addMeter(float value);
	void				subtractMeter(float value);
	void				setPosition(sf::Vector2f position);
	void				setPosition(float x, float y);
	void				setFacing(Facing facing);
	void				setPosture(Posture posture);
	void				setActionState(ActionState actionState);

private:
	Type				mType;
	Animation			mSprite;

	float				mHealth;
	float				mMeter;
	sf::Vector2f		mPosition;
	Facing				mFacing;
	Posture				mPosture;
	ActionState			mActionState;
};