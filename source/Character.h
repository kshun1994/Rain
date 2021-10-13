#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Animation.h"
#include "CommandQueue.h"
#include "Input.h"
#include "Player.h"
#include "Action.h"
#include "InputTrigger.h"
#include "Box.h"

#include <SFML/Graphics/Sprite.hpp>

class Character : public Entity
{
public:
	typedef std::pair<std::unique_ptr<Action>, bool> ActionPair;

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

		std::vector<int>	standIDs;
		std::vector<int>	standDurs;

		std::vector<int>	crouchIDs;
		std::vector<int>	crouchDurs;

		std::vector<int>	crouchToStandIDs;
		std::vector<int>	crouchToStandDurs;

		std::vector<int>	fWalkIDs;
		std::vector<int>	fWalkDurs;

		std::vector<int>	bWalkIDs;
		std::vector<int>	bWalkDurs;

		std::vector<int>	jumpIDs;
		std::vector<int>	jumpDurs;

		std::vector<int>	standBIDs;
		std::vector<int>	standBDurs;
	};

public:
	explicit											Character(Type type, const TextureHolder& textures);
														~Character();

	virtual void										drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void										updateCurrent();
	virtual void										handleInput(Player::TaggedInput input);

	unsigned int										getCategory() const;
	Type												getType() const;

	float												getHealth() const;
	void												setHealth(float value);
	void												subtractHealth(float value);

	float												getMeter() const;
	void												setMeter(float value);
	void												addMeter(float value);
	void												subtractMeter(float value);

	Posture												getPosture() const;
	void												setPosture(Posture posture);

	float												getGravity() const;
	void												setGravity(const float& gravity);

	int													getCurrentActionID();
	void												setCurrentActionID(int id);
	Action*												getCurrentAction() const;

	void												setAnimationFrames(const std::vector<int>& frameIDs,
																		   const std::vector<int>& durations,
																 		   const sf::Vector2i& rect);
	void												setAnimationRepeat(bool flag);
	void												setCurrentAnimationTick(const int& tick);
	void												setCurrentAnimationFrame(const int& frame);

private:
	void												parseInput(unsigned int input);
	void												clearActions();

private:
	Type												type_;
	SpriteStruct										spriteStruct_;

	Action*												action_;
	std::vector<ActionPair>								actions_;
	int													actionID_;

	std::vector<std::unique_ptr<InputTrigger>>			inputTriggers_;

	std::map<int, bool>									stateMap_;

	float												health_;
	float												meter_;
	Facing												facing_;
	Posture												posture_;
	float												gravity_;
};

#define CONTINUE_ACTION					-1
#define COMMON_ACTION_STAND				 0
#define COMMON_ACTION_F_WALK			 1
#define COMMON_ACTION_B_WALK			 2
#define COMMON_ACTION_CROUCH			 3
#define COMMON_ACTION_BLOCK_STAND		 4
#define COMMON_ACTION_BLOCK_CROUCH		 5
#define COMMON_ACTION_F_JUMP			 6
#define COMMON_ACTION_B_JUMP			 7
#define COMMON_ACTION_N_JUMP			 8
#define COMMON_ACTION_LAND_RECOVER		 9
#define COMMON_ACTION_QCF				10
#define COMMON_ACTION_BACK_CHARGE		11
#define COMMON_ACTION_5B				12
#define COMMON_ACTION_TEST				13