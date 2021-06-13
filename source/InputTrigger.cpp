#include "rnpch.h"
#include "InputTrigger.h"
#include "Input.h"

InputTrigger::InputTrigger()
	: trigger_(false)
	, buffer_(0)
	, timer_(0)
	, input_()
	, progressIndex_(0)
	, isButtonInput_()
	, fuzzyInputFlags_(false)
	, fuzzyMapping_()
{
}

//InputTrigger::InputTrigger(std::string input, int buffer)
//	: trigger_(false)
//	, buffer_(buffer)
//	, timer_(0)
//	, input_(input)
//	, progressIndex_(0)
//	, isButtonInput_(input.size(), false)
//	, fuzzyInputFlags_(input.size(), false)
//	, fuzzyMapping_{{0, false}}
//{
//}

bool InputTrigger::isTriggered()
{
	return trigger_;
}

void InputTrigger::setBuffer(int buffer)
{
	buffer_ = buffer;
}

void InputTrigger::setInput(std::string input)
{
	input_.resize(input.size());
	isButtonInput_.resize(input.size());

	// The beginning of an InputTrigger cannot be the release of a previous button (though that sounds interestingly cursed)
	assert(input[0] != 'X'); 

	for (int i = 0; i < input.size(); i++)
	{
		assert(isdigit(input[i]) | isalpha(input[i])); // If this assert triggers it means a non-alphanumeric character was passed as an input
		char inputChar = input[i];

		if (isalpha(input[i]))
		{
			isButtonInput_[i] = true;
		}
		
		// Making this pass inputChar instead of input[i] avoids complications in the event of a repeated button input
		input_[i] = numpadToCharInput(inputChar); 
	}

	// All inputs are non-fuzzy by default
	fuzzyInputFlags_.resize(input.size());
	std::fill(fuzzyInputFlags_.begin(), fuzzyInputFlags_.end(), false);
}

void InputTrigger::setCharge(int chargeDuration)
{
	chargeDuration -= 1; // - 1 because the input should already include one instance of the held input
	// Duplicate held input for as many frames as charge duration requires
	std::vector charge(chargeDuration, input_.front());
	input_.insert(input_.begin() + 1, charge.begin(), charge.end());

	// Current format allows for indefinite resetting of buffer timer as long as the initial charge has been done

	// Add charge duration to buffer for sake of timer
	buffer_ += chargeDuration;
}

void InputTrigger::setCharge(int chargeDuration, std::vector<bool> fuzzyFlags)
{
	assert(input_.size() != 0); // If this assertion pops you tried to set a charge before specifying the input input
	assert(fuzzyFlags.size() == input_.size()); // fuzzyFlags should be a vector the same length as the original input input (e.g. {true, false} for sonic boom {4, 6} input)

	std::transform(input_.begin(), input_.end(), fuzzyFlags.begin(), std::inserter(fuzzyMapping_, fuzzyMapping_.end()), std::make_pair<int const&, bool const&>);

	chargeDuration -= 1; // - 1 because the input should already include one instance of the held input

	// Duplicate held input for as many frames as charge duration requires
	// For now InputTrigger only supports charge inputs where the first input is the one that is held
	std::vector charge(chargeDuration, input_.front());
	input_.insert(input_.begin() + 1, charge.begin(), charge.end());

	// Add charge duration to buffer for sake of timer
	buffer_ += chargeDuration;

	fuzzyInputFlags_ = fuzzyFlags;
	std::vector chargeFuzzyFlags(chargeDuration, fuzzyFlags.front());
	fuzzyInputFlags_.insert(fuzzyInputFlags_.begin() + 1, chargeFuzzyFlags.begin(), chargeFuzzyFlags.end());
}

int InputTrigger::getBuffer()
{
	return buffer_;
}

int InputTrigger::getTimer()
{
	return timer_;
}

std::vector<int> InputTrigger::getInput()
{
	return input_;
}

void InputTrigger::update(int playerInput)
{
	// If input was triggered last update, reset
	if (trigger_)
	{
		trigger_ = false;
	}

	// If there's a fuzzy mapping, convert relevant diagonal inputs to cardinals for input reading
	// Note: current implementation means there's no way to make an input "unfuzzy" once registered in the mapping
	//for (auto it = fuzzyMapping_.begin(); it != fuzzyMapping_.end(); it++)
	//{
	//	switch (it->first)
	//	{
	//	case 4:
	//		input = ((input == 1) | (input == 7)) ? 4 : input;
	//		break;

	//	case 6:
	//		input = ((input == 3) | (input == 9)) ? 6 : input;
	//		break;

	//	case 2:
	//		input = ((input == 1) | (input == 3)) ? 2 : input;
	//		break;

	//	case 8:
	//		input = ((input == 7) | (input == 9)) ? 8 : input;
	//		break;
	//	}
	//}

	if (input_[progressIndex_] == CharInput::CharRelease)
	{
		// Make sure button from previous input has been released before progressing further in input
		if (!(playerInput & input_[progressIndex_ - 1]))
		{
			progressIndex_++;
		}
	}

	// If the current part of the input is neither a button input nor a fuzzy direction input, check for exact matches in direction input
	if (!isButtonInput_[progressIndex_] & !fuzzyInputFlags_[progressIndex_])
	{
		// If the first input in the input is detected and timer hasn't already been started
		// The timer check ensures inputs can repeat the first input without restarting the timer midway through the input
		if ((timer_ == 0) && ((playerInput & 15) == (input_[0] & 15))) 
		{
			// Start and set timer to be input's buffer duration, minus one to account for current update
			timer_ = buffer_ - 1;

			// There's no need to increment progressIndex_ here because as soon as the timer is started we move into the next if
		}

		if ((timer_ > 0) && ((playerInput & 15) == (input_[progressIndex_] & 15))) 
		{
			// Let InputTrigger know to wait for the next input
			progressIndex_++;
		}
		else if (timer_ == 0)
		{
			// If time runs out before the input completes, reset progress
			progressIndex_ = 0;
		}
	}

	// Making this an if instead of if else allows button portions of inputs to be done on the exact same frame as a preceding directional
	if (progressIndex_ != input_.size())
	{
		if (isButtonInput_[progressIndex_] | fuzzyInputFlags_[progressIndex_]) //Technically this also lets you skip the last motion input if it's fuzzy
		{
			// If the first input in the input is detected and timer hasn't already been started
			// The timer check ensures inputs can repeat the first input without restarting the timer midway through the input
			if ((timer_ == 0) && (playerInput & input_[0])) 
			{
				// Start and set timer to be input's buffer duration, minus one to account for current update
				timer_ = buffer_ - 1;

				// There's no need to increment progressIndex_ here because as soon as the timer is started we move into the next if
			}

			if ((timer_ > 0) && (playerInput & input_[progressIndex_])) 
			{
				// Let InputTrigger know to wait for the next input
				progressIndex_++;
			}
			else if (timer_ == 0)
			{
				// If time runs out before the input completes, reset progress
				progressIndex_ = 0;
			}
		}
	}

	// If full input has been read
	if (progressIndex_ == input_.size())
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
