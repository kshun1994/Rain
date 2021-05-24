#include "rnpch.h"
#include "InputTrigger.h"

InputTrigger::InputTrigger()
	: trigger_(false)
	, buffer_(0)
	, timer_(0)
	, motion_()
	, progressIndex_(0)
	, fuzzyInputFlags_(false)
	, fuzzyMapping_()
{
}

InputTrigger::InputTrigger(std::vector<unsigned int> motion, unsigned int buffer)
	: trigger_(false)
	, buffer_(buffer)
	, timer_(0)
	, motion_(motion)
	, progressIndex_(0)
	, fuzzyInputFlags_(motion.size(), false)
	, fuzzyMapping_{{0, false}}
{
}

bool InputTrigger::isTriggered()
{
	return trigger_;
}

void InputTrigger::setBuffer(unsigned int buffer)
{
	buffer_ = buffer;
}

void InputTrigger::setMotion(std::vector<unsigned int> motion)
{
	motion_ = motion;

	// All inputs are non-fuzzy by default
	fuzzyInputFlags_.resize(motion.size());
	std::fill(fuzzyInputFlags_.begin(), fuzzyInputFlags_.end(), false);
}

void InputTrigger::setCharge(unsigned int chargeDuration)
{
	// Duplicate held input for as many frames as charge duration requires
	std::vector charge(chargeDuration - 1, motion_.front()); // - 1 because the motion should already include one instance of the held input
	motion_.insert(motion_.begin() + 1, charge.begin(), charge.end());

	// Current format allows for indefinite resetting of buffer timer as long as the initial charge has been done

	// Add charge duration to buffer for sake of timer
	buffer_ += chargeDuration;
}

void InputTrigger::setCharge(unsigned int chargeDuration, std::vector<bool> fuzzyFlags)
{
	assert(motion_.size() != 0); // If this assertion pops you tried to set a charge before specifying the motion input
	assert(fuzzyFlags.size() == motion_.size()); // fuzzyFlags should be a vector the same length as the original motion input (e.g. {true, false} for sonic boom {4, 6} motion)

	std::transform(motion_.begin(), motion_.end(), fuzzyFlags.begin(), std::inserter(fuzzyMapping_, fuzzyMapping_.end()), std::make_pair<unsigned int const&, bool const&>);

	// Duplicate held input for as many frames as charge duration requires
	// For now InputTrigger only supports charge inputs where the first input is the one that is held
	std::vector charge(chargeDuration - 1, motion_.front()); // - 1 because the motion should already include one instance of the held input
	motion_.insert(motion_.begin() + 1, charge.begin(), charge.end());

	// Add charge duration to buffer for sake of timer
	buffer_ += chargeDuration;

	fuzzyInputFlags_ = fuzzyFlags;
	std::vector chargeFuzzyFlags(chargeDuration - 1, fuzzyFlags.front());
	fuzzyInputFlags_.insert(fuzzyInputFlags_.begin() + 1, chargeFuzzyFlags.begin(), chargeFuzzyFlags.end());
}

unsigned int InputTrigger::getBuffer()
{
	return buffer_;
}

unsigned int InputTrigger::getTimer()
{
	return timer_;
}

std::vector<unsigned int> InputTrigger::getMotion()
{
	return motion_;
}

void InputTrigger::update(unsigned int playerInput)
{
	// If input was triggered last update, reset
	if (trigger_)
	{
		trigger_ = false;
	}

	// (input & 15) is a bitmask to filter player input to first four bits; i.e., the directional input [in numpad notation]
	unsigned int input = playerInput & 15;

	// If there's a fuzzy mapping, convert relevant diagonal inputs to cardinals for input reading
	// Note: current implementation means there's no way to make an input "unfuzzy" once registered in the mapping
	for (auto it = fuzzyMapping_.begin(); it != fuzzyMapping_.end(); it++)
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
	if ((timer_ == 0) && (input == motion_[0])) // ((input.first == motion_[0]) | (input.second == motion_[0])))
	{
		// Start and set timer to be input's buffer duration, minus one to account for current update
		timer_ = buffer_;
	}

	if ((timer_ > 0) && (input == motion_[progressIndex_])) // ((input.first == motion_[progressIndex_]) | (input.second == motion_[progressIndex_])))
	{
		// Let InputTrigger know to wait for the next input
		progressIndex_++;
	}
	else if (timer_ == 0)
	{
		// If time runs out before the input completes, reset progress
		progressIndex_ = 0;
	}

	// If full input has been read
	if (progressIndex_ == motion_.size())
	{
		// Signal that input has been triggered
		trigger_ = true;

		// Reset variables
		progressIndex_ = 0;
		timer_ = 0;
	}
		 
	// Decrement timer
	if (timer_ > 0)
	{
		timer_--;
	}
}
