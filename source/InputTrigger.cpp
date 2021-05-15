#include "rnpch.h"
#include "InputTrigger.h"

InputTrigger::InputTrigger()
	: mTrigger(false)
	, mBuffer(0)
	, mTimer(0)
	, mMotion()
	, mProgressIndex(0)
	, mFuzzyInputFlags(false)
	, mFuzzyMapping()
{
}

InputTrigger::InputTrigger(std::vector<unsigned int> motion, unsigned int buffer)
	: mTrigger(false)
	, mBuffer(buffer)
	, mTimer(0)
	, mMotion(motion)
	, mProgressIndex(0)
	, mFuzzyInputFlags(motion.size(), false)
	, mFuzzyMapping{{0, false}}
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

void InputTrigger::setMotion(std::vector<unsigned int> motion)
{
	mMotion = motion;

	// All inputs are non-fuzzy by default
	mFuzzyInputFlags.resize(motion.size());
	std::fill(mFuzzyInputFlags.begin(), mFuzzyInputFlags.end(), false);
}

void InputTrigger::setCharge(unsigned int chargeDuration)
{
	// Duplicate held input for as many frames as charge duration requires
	std::vector charge(chargeDuration - 1, mMotion.front()); // - 1 because the motion should already include one instance of the held input
	mMotion.insert(mMotion.begin() + 1, charge.begin(), charge.end());

	// Current format allows for indefinite resetting of buffer timer as long as the initial charge has been done

	// Add charge duration to buffer for sake of timer
	mBuffer += chargeDuration;
}

void InputTrigger::setCharge(unsigned int chargeDuration, std::vector<bool> fuzzyFlags)
{
	assert(mMotion.size() != 0); // If this assertion pops you tried to set a charge before specifying the motion input
	assert(fuzzyFlags.size() == mMotion.size()); // fuzzyFlags should be a vector the same length as the original motion input (e.g. {true, false} for sonic boom {4, 6} motion)

	std::transform(mMotion.begin(), mMotion.end(), fuzzyFlags.begin(), std::inserter(mFuzzyMapping, mFuzzyMapping.end()), std::make_pair<unsigned int const&, bool const&>);

	// Duplicate held input for as many frames as charge duration requires
	// For now InputTrigger only supports charge inputs where the first input is the one that is held
	std::vector charge(chargeDuration - 1, mMotion.front()); // - 1 because the motion should already include one instance of the held input
	mMotion.insert(mMotion.begin() + 1, charge.begin(), charge.end());

	// Add charge duration to buffer for sake of timer
	mBuffer += chargeDuration;

	mFuzzyInputFlags = fuzzyFlags;
	std::vector chargeFuzzyFlags(chargeDuration - 1, fuzzyFlags.front());
	mFuzzyInputFlags.insert(mFuzzyInputFlags.begin() + 1, chargeFuzzyFlags.begin(), chargeFuzzyFlags.end());
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
	return mMotion;
}

void InputTrigger::update(unsigned int playerInput)
{
	// If input was triggered last update, reset
	if (mTrigger)
	{
		mTrigger = false;
	}

	// (input & 15) is a bitmask to filter player input to first four bits; i.e., the directional input [in numpad notation]
	unsigned int input = playerInput & 15;

	// If there's a fuzzy mapping, convert relevant diagonal inputs to cardinals for input reading
	// Note: current implementation means there's no way to make an input "unfuzzy" once registered in the mapping
	for (auto it = mFuzzyMapping.begin(); it != mFuzzyMapping.end(); it++)
	{
		switch (it->first)
		{
		case 4:
			input = ((input == 1) | (input == 7)) ? 4 : input;
			break;

		case 6:
			input = ((input == 3) | (input == 9)) ? 6 : input;
			break;

		case 2:
			input = ((input == 1) | (input == 3)) ? 2 : input;
			break;

		case 8:
			input = ((input == 7) | (input == 9)) ? 8 : input;
			break;
		}
	}

	// If the first input in the motion is detected and timer hasn't already been started
	// The timer check ensures motions can repeat the first input and the timer won't be restarted midway through the input
	if ((mTimer == 0) && (input == mMotion[0])) // ((input.first == mMotion[0]) | (input.second == mMotion[0])))
	{
		// Start and set timer to be input's buffer duration, minus one to account for current update
		mTimer = mBuffer;
	}

	if ((mTimer > 0) && (input == mMotion[mProgressIndex])) // ((input.first == mMotion[mProgressIndex]) | (input.second == mMotion[mProgressIndex])))
	{
		// Let InputTrigger know to wait for the next input
		mProgressIndex++;
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
