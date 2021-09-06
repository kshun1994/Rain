#pragma once

class Character;


/*

Characters have vectors of CharState objects
CharStates are allocated during game load (Character initialization)
CharStates are entered via player inputs
CharStates may have durations or may be indefinite until another CharState is entered
CharStates can limit possible CharState changes
	Hitstun/blockstun etc. doesn't allow for actions
	Specific moves might be able to cancel into specific others
Map of inputs to CharStates that can be filtered?
	E.g. if in recovery state limit entry into states other than hitstun/blockstun etc.
	Maybe use bit flag enum to categorize CharStates

*/

class CharState
{
public: 
	virtual								~CharState() {};

	virtual void						update(Character& character) {};
	virtual std::shared_ptr<CharState>	handleInput(Character& character, std::map<int, bool> stateMap);

	virtual void						enter(Character& character) {};

	virtual void						setAnimationFrames(const std::vector<int>& frameIDs,
														   const std::vector<int>& durations,
														   const sf::Vector2i& spriteDims);
	virtual void						setAnimationRepeat(bool flag);

protected:
	virtual void						setAnimation(Character& character);

protected:
	std::vector<int>					animationFrameIDs_;
	std::vector<int>					animationFrameDurations_;
	sf::Vector2i						animationSpriteDims_;
	bool								animationIsRepeating_;
};

class StandState : public CharState
{
public:
	virtual								~StandState() {};

	virtual void						enter(Character& character);

};

class CrouchState : public CharState
{
public:
	virtual								~CrouchState() {};

	virtual void						enter(Character& character);

};

class FWalkState : public CharState
{
public:
	virtual								~FWalkState() {};

	virtual void						update(Character& character);

	virtual void						enter(Character& character);

	void								setSpeed(float speed);

private:
	float								speed_ = 5.f;
};

class BWalkState : public CharState
{
public:
	virtual								~BWalkState() {};

	virtual void						update(Character& character);

	virtual void						enter(Character& character);

	void								setSpeed(float speed);

private:
	float								speed_ = 5.f;
};
