// Instantiate an InputTrigger per motion input available to a character before a match begins
// InputTriggers will "listen" to inputs every update
//     mTimer is the buffer duration for the move
//     If an input on a given update matches the first required input for a motion, the object moves on to the next input in line
//	   mTimer decrements by 1 every update. If mTimer reaches 0, undo all progres
//	   If progress reaches end of motion before mTimer reachers 0, flip mTrigger---InputTrigger::isTriggered() method informs that the motion input has triggered
// Fuzzy inputs: if an input is specified as a fuzzy input, diagonals will count as both cardinals for the sake of input detection (e.g. you can hold down-back and it'll register back charge)
#pragma once


class InputTrigger
{
public:
									InputTrigger();
									InputTrigger(std::vector<unsigned int> motion, unsigned int buffer); // Takes directional motion in numpad notation and a buffer duration

public:
	bool							isTriggered();

	void							setBuffer(unsigned int buffer);
	void							setMotion(std::vector<unsigned int> motion);
	void							setCharge(unsigned int chargeDuration);
	void							setCharge(unsigned int chargeDuration, std::vector<bool> fuzzyFlags);

	unsigned int					getBuffer();
	unsigned int					getTimer();
	std::vector<unsigned int>		getMotion();

	void							update(unsigned int playerIinput);

private:
	bool							mTrigger;

	unsigned int					mBuffer;
	unsigned int					mTimer;
	std::vector<unsigned int>		mMotion;
	std::vector<bool>				mFuzzyInputFlags;
	std::map<unsigned int, bool>	mFuzzyMapping;

	unsigned int					mProgressIndex;
};

