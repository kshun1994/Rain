#pragma once

#include "Box.h"

class Character;


/*

Characters have vectors of ActionState objects
ActionStates are allocated during game load (Character initialization)
ActionStates are entered via player inputs
ActionStates may have durations or may be indefinite until another ActionState is entered
ActionStates can limit possible ActionState changes
	Hitstun/blockstun etc. doesn't allow for actions
	Specific moves might be able to cancel into specific others
Map of inputs to ActionStates that can be filtered?
	E.g. if in recovery state limit entry into states other than hitstun/blockstun etc.
	Maybe use bit flag enum to categorize ActionStates

*/

class ActionState
{
public: 
	virtual								~ActionState() {};

	virtual void						update(Character& character) {};
	virtual int							handleInput(Character& character, std::map<int, bool> stateMap);

	virtual void						enter(Character& character);

	virtual void						setAnimationFrames(const std::vector<int>& frameIDs,
														   const std::vector<int>& durations,
														   const sf::Vector2i& spriteDims);
	virtual void						setAnimationRepeat(bool flag);

	virtual void						setBoxes(std::vector<std::shared_ptr<Box>> boxes);
	virtual void						appendBox(std::shared_ptr<Box> box);

	virtual void						setFrameData(const int& startup, const int& active, const int& recovery);

protected:
	virtual void						setAnimation(Character& character);

protected:
	std::vector<int>					animationFrameIDs_;
	std::vector<int>					animationFrameDurations_;
	sf::Vector2i						animationSpriteDims_;
	bool								animationIsRepeating_;

	std::vector<std::shared_ptr<Box>>	boxes_;
	std::vector<Box*>					boxPtrs_;

protected:
	int									startup_;
	int									active_;
	int									recovery_;

	int									progress_;
};

class StandState : public ActionState
{
public:
	virtual								~StandState() {};

	virtual void						enter(Character& character);

};

class CrouchState : public ActionState
{
public:
	virtual								~CrouchState() {};

	virtual void						enter(Character& character);

};

class FWalkState : public ActionState
{
public:
	virtual								~FWalkState() {};

	virtual void						update(Character& character);

	virtual void						enter(Character& character);

	void								setSpeed(float speed);

private:
	float								speed_ = 5.f;
};

class BWalkState : public ActionState
{
public:
	virtual								~BWalkState() {};

	virtual void						update(Character& character);

	virtual void						enter(Character& character);

	void								setSpeed(float speed);

private:
	float								speed_ = 5.f;
};
