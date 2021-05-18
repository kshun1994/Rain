#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"
#include "Input.h"
#include "Player.h"

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
		Left	= Input::Left,
		Right	= Input::Right,
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
						~Character();

	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void		updateCurrent();
	virtual void		takeInput(Player::TaggedInput input);

	unsigned int		getCategory() const;

	float				getHealth() const;
	float				getMeter() const;
	Facing				getFacing() const;
	Posture				getPosture() const;
	ActionState			getActionState() const;
	bool				isActionable() const;

	void				setHealth(float value);
	void				subtractHealth(float value);
	void				setMeter(float value);
	void				addMeter(float value);
	void				subtractMeter(float value);
	void				setFacing(Facing facing);
	void				flipFacing();
	void				setPosture(Posture posture);
	void				setActionState(ActionState actionState);

private:
	Type				mType;
	Animation			mSprite;

	float				mHealth;
	float				mMeter;
	Facing				mFacing;
	Posture				mPosture;
	ActionState			mActionState;
};

#define COMMON_ACTION_IDLE				0
#define COMMON_ACTION_WALK_FORWARD		1
#define COMMON_ACTION_WALK_BACK			2
#define COMMON_ACTION_CROUCH			3
#define COMMON_ACTION_BLOCK_STANDING	4
#define COMMON_ACTION_BLOCK_CROUCHING	5
#define COMMON_ACTION_JUMP				6
