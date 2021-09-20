#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"
#include "Input.h"
#include "Player.h"
#include "CharState.h"
#include "InputTrigger.h"
#include "Box.h"

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
		float				originX;
		float				originY;

		std::vector<int>	idleIDs;
		std::vector<int>	idleDurs;

		std::vector<int>	crouchIDs;
		std::vector<int>	crouchDurs;

		std::vector<int>	fWalkIDs;
		std::vector<int>	fWalkDurs;

		std::vector<int>	bWalkIDs;
		std::vector<int>	bWalkDurs;
	};

	//enum BoxType
	//{
	//	Collide			= 1 << 0, 
	//	Hit				= 1 << 1,
	//	Hurt			= 1 << 2,
	//};

	//struct Box
	//{
	//	int					boxType;
	//	sf::IntRect			box;
	//};

public:
	explicit											Character(Type type, const TextureHolder& textures);
														~Character();

	virtual void										drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void										updateCurrent();
	virtual void										handleInput(Player::TaggedInput input);

	unsigned int										getCategory() const;

	float												getHealth() const;
	float												getMeter() const;
	Facing												getFacing() const;
	Posture												getPosture() const;
	ActionState											getActionState() const;
	bool												isActionable() const;

	void												setHealth(float value);
	void												subtractHealth(float value);
	void												setMeter(float value);
	void												addMeter(float value);
	void												subtractMeter(float value);
	void												setFacing(Facing facing);
	void												flipFacing();
	void												setPosture(Posture posture);
	void												setActionState(ActionState actionState);

	//std::vector<std::unique_ptr<CharState>>				getCharStates();
	//std::unique_ptr<CharState>							getCurrentCharState();
	int													getCurrentCharStateID();
	void												setCurrentCharStateID(int id);

	void												setAnimationFrames(const std::vector<int>& frameIDs,
																		   const std::vector<int>& durations,
																 		   const sf::Vector2i& rect);
	void												setAnimationRepeat(bool flag);

	float												getFacingSign();

	void												detachBoxes();

private:
	void												setSignFlip();
	void												parseInput(unsigned int input);
	void												clearStateMap();

private:
	Type												type_;
	Animation											sprite_;
	SpriteStruct										spriteStruct_;

	CharState*											charState_;
	std::vector<std::unique_ptr<CharState>>				charStates_;
	int													charStateID_;

	std::vector<std::unique_ptr<InputTrigger>>			inputTriggers_;

	std::map<int, bool>									stateMap_;

	//std::vector<Box>									boxes_;
	//std::unique_ptr<Box>								collideBox_;

	AnimationState										animationState_;
	AnimationState										prevAnimationState_;

	float												health_;
	float												meter_;
	Facing												facing_;
	Posture												posture_;
	ActionState											actionState_;
	float												facingSign_;
};

#define NULL_ACTION						 0
#define COMMON_ACTION_STAND				 1
#define COMMON_ACTION_F_WALK			 2
#define COMMON_ACTION_B_WALK			 3
#define COMMON_ACTION_CROUCH			 4
#define COMMON_ACTION_BLOCK_STAND		 5
#define COMMON_ACTION_BLOCK_CROUCH		 6
#define COMMON_ACTION_JUMP				 7
#define COMMON_ACTION_QCF				 8
#define COMMON_ACTION_BACK_CHARGE		 9
