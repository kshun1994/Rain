#include "rnpch.h"
#include "InputTrigger.h"

InputTrigger::InputTrigger()
	: mTrigger(false)
	, mBuffer(0)
	, mTimer(0)
	, mMotion()
	, mProgressIndex(0)
{
}

InputTrigger::InputTrigger(std::vector<unsigned int> motion, unsigned int buffer)
	: mTrigger(false)
	, mBuffer(buffer)
	, mTimer(0)
	, mMotion(motion)
	, mProgressIndex(0)
{
}

bool InputTrigger::isTriggered()
{
	return mTrigger;
}

void InputTrigger::setBuffer(unsigned int buffer)
{
	mBuffer = buffer;
}

void InputTrigger::setTimer(unsigned int timer)
{
	// I can't imagine why you'd ever use this function but why not
	mTimer = timer;
}

void InputTrigger::setMotion(std::vector<unsigned int> motion)
{
	mMotion = motion;
}

unsigned int InputTrigger::getBuffer()
{
	return mBuffer;
}

unsigned int InputTrigger::getTimer()
{
	return mTimer;
}

std::vector<unsigned int> InputTrigger::getMotion()
{
	// Again, not really sure what you'd use this for
	return mMotion;
}

void InputTrigger::update(unsigned int input)
{
	// If input was triggered last update, reset
	if (mTrigger)
	{
		mTrigger = false;
	}

	// (input & 15) is a bitmask to filter player input to first four bits; i.e., the directional input [in numpad notation]
	if (mTimer > 0 && ((input & 15) == mMotion[mProgressIndex]))
	{
		// Let InputTrigger know to wait for the next input
		mProgressIndex++;
	}
	// If the first input in the motion is detected and timer hasn't already been started
	// The timer check ensures motions can repeat the first input and the timer won't be restarted midway through the input
	else if (mTimer == 0 && ((input & 15) == mMotion[0]))
	{
		// Start and set timer to be input's buffer duration, minus one to account for current update
		mTimer = mBuffer;
	}
	else if (mTimer == 0)
	{
		// If time runs out before the input completes, reset progress
		mProgressIndex = 0;
	}

	// If full input has been read
	if (mProgressIndex == mMotion.size())
	{
		// Signal that input has been triggered
		mTrigger = true;

		// Reset variables
		mProgressIndex = 0;
		mTimer = 0;
	}
		 
	// Decrement timer
	if (mTimer > 0)
	{
		mTimer--;
	}
}
