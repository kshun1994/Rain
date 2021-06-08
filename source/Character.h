#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"
#include "Input.h"
#include "Player.h"
#include "CharState.h"
#include "InputTrigger.h"
#include "StateStack.h"

#include <memory>

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

	enum class AnimationState
	{
		Idle,
		WalkF, 
		WalkB,
	};

	struct SpriteStruct
	{
		sf::Vector2i		spriteDims;
		int					originX;

		std::vector<int>	idleIDs;
		std::vector<int>	idleDurs;

		std::vector<int>	crouchIDs;
		std::vector<int>	crouchDurs;

		std::vector<int>	walkFIDs;
		std::vector<int>	walkFDurs;

		std::vector<int>	walkBIDs;
		std::vector<int>	walkBDurs;
	};

public:
	explicit										Character(Type type, const TextureHolder& textures);
													~Character();

	virtual void									drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void									updateCurrent();
	virtual void									handleInput(int input);

	int												getCategory() const;

	float											getHealth() const;
	float											getMeter() const;
	Facing											getFacing() const;
	Posture											getPosture() const;
	ActionState										getActionState() const;
	bool											isActionable() const;

	void											setHealth(float value);
	void											subtractHealth(float value);
	void											setMeter(float value);
	void											addMeter(float value);
	void											subtractMeter(float value);
	void											setFacing(Facing facing);
	void											flipFacing();
	void											setPosture(Posture posture);
	void											setActionState(ActionState actionState);

	void											walkForward(float speed);
	void											walkBackward(float speed);

	void											setAnimation(const std::vector<int>& ids, 
																 const std::vector<int>& durations, 
																 const sf::Vector2i& spriteDims, 
																 const bool& isRepeating = false);
	void											setAnimation(const std::vector<sf::IntRect>& frameRects, 
																 const std::vector<int>& durations,
																 const bool& isRepeating = false);
	int												getCharInput() const;

private:
	void											setSignFlip();
	void											calculateCharInput(int input);

private:
	Type											type_;
	Animation										sprite_;
	SpriteStruct									spriteStruct_;

	StateStack										charStateStack_;

	AnimationState									animationState_;
	AnimationState									prevAnimationState_;

	float											health_;
	float											meter_;
	Facing											facing_;
	Posture											posture_;
	ActionState										actionState_;
	int												facingSignFlip_;

	int												charInput_;

	std::vector<std::unique_ptr<InputTrigger>>		triggers_;
};

#define COMMON_ACTION_IDLE				0
#define COMMON_ACTION_WALK_FORWARD		1
#define COMMON_ACTION_WALK_BACK			2
#define COMMON_ACTION_CROUCH			3
#define COMMON_ACTION_BLOCK_STANDING	4
#define COMMON_ACTION_BLOCK_CROUCHING	5
#define COMMON_ACTION_JUMP				6
