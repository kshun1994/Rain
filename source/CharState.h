#pragma once

class Character;

class CharState
{
public:
	virtual											~CharState() {};
	virtual void									update(Character& character) {};
	virtual std::unique_ptr<CharState>				handleInput(Character& character, int input) { return nullptr; };
};

class AnimatedState : public CharState
{
public:
	virtual void									update(Character& character) override;
	virtual std::unique_ptr<CharState>				handleInput(Character& character, int input) override;

	virtual void									setAnimationFrames(std::vector<int> ids, std::vector<int> durations);
	virtual void									setDuration(int duration);
	virtual void									setRepeating(bool flag);

protected:
	int												duration_;
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
