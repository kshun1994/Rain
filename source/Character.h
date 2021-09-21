#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"
#include "Input.h"
#include "Player.h"
#include "ActionState.h"
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

	enum Posture
	{
		Standing,
		Crouching,
		Airborne,
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

public:
	explicit											Character(Type type, const TextureHolder& textures);
														~Character();

	virtual void										drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void										updateCurrent();
	virtual void										handleInput(Player::TaggedInput input);

	unsigned int										getCategory() const;

	float												getHealth() const;
	void												setHealth(float value);
	void												subtractHealth(float value);

	float												getMeter() const;
	void												setMeter(float value);
	void												addMeter(float value);
	void												subtractMeter(float value);

	Posture												getPosture() const;
	void												setPosture(Posture posture);

	int													getCurrentActionStateID();
	void												setCurrentActionStateID(int id);

	void												setAnimationFrames(const std::vector<int>& frameIDs,
																		   const std::vector<int>& durations,
																 		   const sf::Vector2i& rect);
	void												setAnimationRepeat(bool flag);

private:
	void												parseInput(unsigned int input);
	void												clearStateMap();

private:
	Type												type_;
	SpriteStruct										spriteStruct_;

	ActionState*											actionState_;
	std::vector<std::unique_ptr<ActionState>>				actionStates_;
	int													actionStateID_;

	std::vector<std::unique_ptr<InputTrigger>>			inputTriggers_;

	std::map<int, bool>									stateMap_;

	float												health_;
	float												meter_;
	Facing												facing_;
	Posture												posture_;
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
