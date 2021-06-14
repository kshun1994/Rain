#pragma once

class Character;

class CharState
{
public: 
	virtual								~CharState() {};

	virtual CharState*					handleInput(Character& character, int input) { return nullptr; };
	virtual void						update(Character& character) {};

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

	virtual CharState*					handleInput(Character& character, int input);
	virtual void						update(Character& character);

	virtual void						enter(Character& character);

};

class CrouchState : public CharState
{
public:
	virtual								~CrouchState() {};

	virtual CharState*					handleInput(Character& character, int input);
	virtual void						update(Character& character);

	virtual void						enter(Character& character);

};

