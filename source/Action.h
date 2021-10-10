#pragma once

#include "Box.h"

class Character;


/*

Characters have vectors of Action objects
Actions are allocated during game load (Character initialization)
Actions are entered via player inputs
Actions may have durations or may be indefinite until another Action is entered
Actions can limit possible Action changes
	Hitstun/blockstun etc. doesn't allow for actions
	Specific moves might be able to cancel into specific others
Map of inputs to Actions that can be filtered?
	E.g. if in recovery state limit entry into states other than hitstun/blockstun etc.
	Maybe use bit flag enum to categorize Actions

*/

/* 

How to create an Action during character initialization?

Instatiate action
	Specify startup/active/recovery in constructor
	Moves that have multiple sets of active frames need to be specified differently?
	E.g. set up a vector of frame numbers and an accompanying vector of identifiers
	Have an enum Action::Property that includes startup/active/recovery but also 
	  stuff like throw invul/airborne etc; property_ member var that's an unsigned int, holds multiple properties at once
Set animation 
	A vector of either spritesheet indices or rectangle coordinates
	A vector of animation frame durations
	The sizes of the above vectors need to equal the total duration (startup + active + recovery)


Need some kind of updateProperties() private function that per-frame checks which properties an action has and 
  adds/removes them from properties_ accordingly
	E.g. properties_ |= Action::Property::Airborne;
	Have a member vector that one can append pairs of Properties and relevant frame indices to
		struct FrameRange {int lowerBound; int upperBound};
		std::vector<std::pair<Action::Property, std::vector<bool>>>		propertyVect_;
		Action->propertyVect_.push_back(std::pair<Action::Property, std::vector<bool>>(Action::Property::Airborne, {true, true, true, false, true}));
		
	

*/

class Action
{
public:
	typedef std::vector<std::shared_ptr<Box>> Boxes;

public:
	struct Ballistics
	{
		float LaunchVelocity;
		float LaunchAngle;
		int	  FrameApplied;
	};

public:
	enum Property
	{
		None				= 0,
		Startup				= 1 << 0,
		Active				= 1 << 1,
		Recovery			= 1 << 2,
		Airborne			= 1 << 3,
		Cancellable			= 1 << 4,
	};

public: 
										Action();
	virtual								~Action() {};

	virtual void						update(Character& character);
	virtual int							handleInput(Character& character, std::map<int, bool> stateMap);

	virtual void						enter(Character& character);

	virtual void						setAnimationFrames(const std::vector<int>& frameIDs,
														   const std::vector<int>& durations,
														   const sf::Vector2i& spriteDims);

	virtual void						setLoopBounds(const int& start, const int& end);
	virtual void						setLoopBounds(const std::pair<int, int>& bounds);

	virtual void						updateBoxes(Character& character);
	virtual void						setBoxes(const int& frame, Boxes boxes);

	virtual void						addProperty(Property property, std::vector<int> frameInds);
	virtual int 						getCurrentProperty() const;

	virtual void						setMovePerFrame(const std::vector<sf::Vector2f>& movePerFrame);

										// For states like airborne (jumping) and hitstun
	virtual void						applyBallisticVector(const float& launchVelocity, const float& launchAngle);

protected:
	virtual sf::Vector2f				calculateVelocity(const float& gravity);

	virtual void						setAnimation(Character& character);
	virtual void						setAnimation(Character& character, const std::vector<int>& frameIDs,
																		   const std::vector<int>& durations,
															               const sf::Vector2i& spriteDims);

protected:
	std::vector<int>					animationFrameIDs_;
	std::vector<int>					animationLoopFrames_;
	std::vector<int>					animationFrameDurations_;
	sf::Vector2i						animationSpriteDims_;
	bool								animationDoesLoop_;
	bool								animationIsLooping_;

	std::vector<sf::Vector2f>			velocityPerFrame_;
	std::vector<Ballistics>				ballistics_;

	std::pair<int, int>					loopBounds_;
	int									nextLoopBound_;

	std::vector<Boxes>					boxes_;
	std::vector<Box*>					boxPtrs_;
	int									currentBoxesInd_;

	std::vector<int>					properties_;

	int									currentFrame_;
};

class AirborneAction : public Action
{
public:
										AirborneAction();
										~AirborneAction();

	virtual void						update(Character& character);
	virtual int							handleInput(Character& character, std::map<int, bool> stateMap);

	virtual void						enter(Character& character);

	virtual void						setBoxes(const int& frame, Boxes boxes);

protected:				
	virtual sf::Vector2f				calculateVelocity(const float& gravity);

protected:
	int									startup_;
};

class JumpAction : public AirborneAction
{
public:
	virtual								~JumpAction() {};

	virtual void						update(Character& character);

	virtual void						setJumpStartup(const int& startup);
	virtual void						setJumpBallistics(const float& launchVelocity, const float& launchAngle);

protected:
	float								launchVelocity_;
	float								launchAngle_;
};
