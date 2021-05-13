#pragma once


class InputTrigger
{
public:
									InputTrigger();
									InputTrigger(std::vector<unsigned int> motion, unsigned int buffer); // Takes directional motion in numpad notation and a buffer duration

public:
	bool							isTriggered();

	void							setBuffer(unsigned int buffer);
	void							setTimer(unsigned int timer);
	void							setMotion(std::vector<unsigned int> motion);

	unsigned int					getBuffer();
	unsigned int					getTimer();
	std::vector<unsigned int>		getMotion();

	void							update(unsigned int input);

private:
	bool							mTrigger;

	unsigned int					mBuffer;
	unsigned int					mTimer;
	std::vector<unsigned int>		mMotion;

	unsigned int					mProgressIndex;
};

// Instantiate an InputTrigger per motion input available to a character before a match begins
// InputTriggers will "listen" to inputs every update
//     mTimer is the buffer duration for the move
//     If an input on a given update matches the first required input for a motion, the first bool in mTracker flips, mTimer is set, and InputTrigger waits for the next input
//     If the next input is read on a following update AND the bool directly preceding the relevant input is true, then flip bool to true
//	   Flipped bools will remain flipped until mTimer runs out
//	   mTimer decrements by 1 every update. If mTimer reaches 0, unflip all bools in mTracker and reset mTimer
//	   If all bools are flipped before mTimer reaches 0, flip mTrigger---InputTrigger.isTriggered() method informs that the motion input has triggered
