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

/* 

How to create an ActionState during character initialization?

Instatiate action
	Specify startup/active/recovery in constructor
	Moves that have multiple sets of active frames need to be specified differently?
	E.g. set up a vector of frame numbers and an accompanying vector of identifiers
	Have an enum ActionState::Property that includes startup/active/recovery but also 
	  stuff like throw invul/airborne etc; property_ member var that's an unsigned int, holds multiple properties at once
Set animation 
	A vector of either spritesheet indices or rectangle coordinates
	A vector of animation frame durations
	The sizes of the above vectors need to equal the total duration (startup + active + recovery)


Need some kind of updateProperties() private function that per-frame checks which properties an action has and 
  adds/removes them from properties_ accordingly
	E.g. properties_ |= ActionState::Property::Airborne;
	Have a member vector that one can append pairs of Properties and relevant frame indices to
		struct FrameRange {int lowerBound; int upperBound};
		std::vector<std::pair<ActionState::Property, std::vector<bool>>>		propertyVect_;
		ActionState->propertyVect_.push_back(std::pair<ActionState::Property, std::vector<bool>>(ActionState::Property::Airborne, {true, true, true, false, true}));
		
	

*/

class ActionState
{
public:
	enum Property
	{
		None				= 0,
		Startup				= 1 << 0,
		Active				= 1 << 1,
		Recovery			= 1 << 2,
		Airborne			= 1 << 3,
	};

public: 
										ActionState();
	virtual								~ActionState() {};

	virtual void						update(Character& character);
	virtual int							handleInput(Character& character, std::map<int, bool> stateMap);

	virtual void						enter(Character& character);

	virtual void						setAnimationFrames(const std::vector<int>& frameIDs,
														   const std::vector<int>& durations,
														   const sf::Vector2i& spriteDims);
	virtual void						setAnimationLoop(const bool& flag);
	virtual void						setAnimationLoop(const std::vector<int>& loopFrames);

	virtual void						setLoopBounds(const int& start, const int& end);
	virtual void						setLoopBounds(const std::pair<int, int>& bounds);

	virtual void						setBoxes(std::vector<std::shared_ptr<Box>> boxes);
	virtual void						appendBox(std::shared_ptr<Box> box);

	virtual void						addProperty(Property property, std::vector<int> frameInds);
	virtual int 						getCurrentProperty() const;

protected:
	virtual void						setAnimation(Character& character);
	virtual void						setAnimation(Character& character, const std::vector<int>& frameIDs,
																		   const std::vector<int>& durations,
															               const sf::Vector2i& spriteDims);
	//virtual void						updateProperties();

protected:
	std::vector<int>					animationFrameIDs_;
	std::vector<int>					animationLoopFrames_;
	std::vector<int>					animationFrameDurations_;
	sf::Vector2i						animationSpriteDims_;
	bool								animationDoesLoop_;
	bool								animationIsLooping_;

	std::pair<int, int>					loopBounds_;

	std::vector<std::shared_ptr<Box>>	boxes_;
	std::vector<Box*>					boxPtrs_;

	std::vector<int>					properties_;

	int									currentFrame_;
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
