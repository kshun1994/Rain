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
	typedef std::pair<int, std::vector<int>> Cancel; // A CancelType (can be multiple) and accompanying vector of cancel-window frames

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

	enum CancelType
	{
		All					=    ~ 0, // Bitwise NOT 0 sets all bits to 1; thus any bitwise AND will return true
		Idle				= 1 << 0,
		Basic				= 1 << 1,
		Normal				= 1 << 2,
		CommandNormal		= 1 << 3,
		Special				= 1 << 4,
		EX					= 1 << 5,
		Super				= 1 << 6,
		Block				= 1 << 7,
		Unique				= 1 << 8, // For if you want to be able to specify that a specific move can be cancelled into; e.g. cancelType_ |= (Action::CancelType::Unique + moveID_);
	};
	// Have a setMoveID() function that internally sets moveID_ as CancelType::Unique + ID 
	// handleInput() will automatically also include potential destination Action's getID() in addition to whether it's a normal/special/etc.

public: 
										Action();
	virtual								~Action() {};

	virtual void						update(Character& character);
	virtual Action*						handleInput(Character& character, std::vector<std::pair<std::unique_ptr<Action>, bool>>& actions);

	virtual void						enter(Character& character);

	virtual void						setAnimationFrames(const std::vector<int>& frameIDs,
														   const std::vector<int>& durations,
														   const sf::Vector2i& spriteDims);

	virtual void						setLoopBounds(const int& start, const int& end);
	virtual void						setLoopBounds(const std::pair<int, int>& bounds);

	virtual void						updateBoxes(Character& character);
	virtual void						setBoxes(const int& frame, Boxes boxes);

	virtual void						addProperty(Property property, std::vector<int> frameInds);
	virtual void						addProperty(Property property, boost::integer_range<int> frameInds);
	virtual int 						getCurrentProperty() const;

	virtual void						setMoveID(const int& moveID);
	virtual int							getMoveID() const;

	virtual void						setCancelType(const int& cancelType);
	virtual int							getCancelType() const;
	virtual void						setCancel(const int& cancelType, const std::vector<int>& frames); 
	virtual void						setCancel(const int& cancelType, const boost::integer_range<int>& frames); 
	virtual void						setCancel(const int& cancelType, const int& startFrameInclusive, const int& endFrameExclusive);

	virtual void						setMovePerFrame(const std::vector<sf::Vector2f>& movePerFrame);

	virtual void						setDestinationAction(Action* action);

										// For states like airborne (jumping) and hitstun
	virtual void						applyBallisticVector(const float& launchVelocity, const float& launchAngle);

protected:
	virtual sf::Vector2f				calculateVelocity(const float& gravity);

	virtual void						setAnimation(Character& character);
	virtual void						setAnimation(Character& character, const std::vector<int>& frameIDs,
																		   const std::vector<int>& durations,
															               const sf::Vector2i& spriteDims);

protected:
	int									currentFrame_;

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

	int									moveID_;
	int									cancelType_;
	std::vector<int>					cancels_;

	Action*								destinationAction_;
};

// For Actions that have a recovery/whatever else that only happens after the player STOPS doing the relevant input
class HeldAction : public Action
{
public:
										~HeldAction();
	virtual Action*						handleInput(Character& character, std::vector<std::pair<std::unique_ptr<Action>, bool>>& actions);
};

// Different from Action in that duration is essentially indefinite until Entity hits the ground
class AirborneAction : public Action
{
public:
										AirborneAction();
										~AirborneAction();

	virtual void						update(Character& character);
	virtual Action*						handleInput(Character& character, std::vector<std::pair<std::unique_ptr<Action>, bool>>& actions);

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
