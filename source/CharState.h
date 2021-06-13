#pragma once

class Character;

class CharState
{
public:
	virtual											~CharState() {};
	virtual void									update(Character& character) {};
	virtual std::unique_ptr<CharState>				handleInput(Character& character, int input) { return nullptr; };
	virtual void									enter(Character& character) {};
	virtual void									exit(Character& character) {};
};

class AnimatedState : public CharState
{
public:
	virtual void									update(Character& character) override;
	virtual std::unique_ptr<CharState>				handleInput(Character& character, int input) override;
	virtual void									enter(Character& character) override;
	virtual void									exit(Character& character) override;

	virtual void									setAnimationParameters(const std::vector<int>& ids, 
																		   const std::vector<int>& durations, 
																		   const sf::Vector2i& spriteDims);
	virtual void									setStateDuration(int duration);
	virtual void									setRepeating(bool flag);

protected:
	std::vector<int>								spriteIds_;
	std::vector<int>								spriteDurations_;
	sf::Vector2i									spriteDims_;

	int												duration_;
	int												progress_;
	bool											isRepeating_;

	std::map<CharState, int>						stateChangeMap_;
};



//class StandState : public CharState
//{
//public:
//	virtual void									enter(Character& character) override;
//	virtual void									update(Character& character) override;
//	virtual std::unique_ptr<CharState>				handleInput(Character& character, int input) override;
//};
//
//class CrouchState : public CharState
//{
//public:
//	virtual void									enter(Character& character) override;
//	virtual void									update(Character& character) override;
//	virtual std::unique_ptr<CharState>				handleInput(Character& character, int input) override;
//};
//
//class AirState : public CharState
//{
//public:
//	virtual void									enter(Character& character) override;
//	virtual void									update(Character& character) override;
//	virtual std::unique_ptr<CharState>				handleInput(Character& character, int input) override;
//};
