// Instantiate an InputTrigger per motion input available to a character before a match begins
// InputTriggers will "listen" to inputs every update
//     mTimer is the buffer duration for the move
//     If an input on a given update matches the first required input for a motion, the object moves on to the next input in line
//	   mTimer decrements by 1 every update. If mTimer reaches 0, undo all progres
//	   If progress reaches end of motion before mTimer reachers 0, flip mTrigger---InputTrigger::isTriggered() method informs that the motion input has triggered
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

