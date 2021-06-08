// Instantiate an InputTrigger per input input available to a character before a match begins
// InputTriggers will "listen" to inputs every update
//     timer_ is the buffer duration for the move
//     If an input on a given update matches the first required input for a input, the object moves on to the next input in line
//	   timer_ decrements by 1 every update. If timer_ reaches 0, undo all progres
//	   If progress reaches end of input before timer_ reachers 0, flip trigger_---InputTrigger::isTriggered() method informs that the input input has triggered
// Fuzzy inputs: if an input is specified as a fuzzy input, diagonals will count as both cardinals for the sake of input detection (e.g. you can hold down-back and it'll register back charge)
#pragma once
#include <string>


class InputTrigger
{
public:
									InputTrigger();
									//InputTrigger(std::vector<int> input, int buffer); // Takes directional input in numpad notation and a buffer duration

public:
	bool							isTriggered();

	void							setBuffer(int buffer);
	void							setInput(std::string input);
	void							setCharge(int chargeDuration);
	void							setCharge(int chargeDuration, std::vector<bool> fuzzyFlags);

	int								getBuffer();
	int								getTimer();
	std::vector<int>				getInput();

	void							update(int playerInput);

private:
	bool							trigger_;

	int								buffer_;
	int								timer_;
	std::vector<int>				input_;
	std::vector<bool>				isButtonInput_;
	std::vector<bool>				fuzzyInputFlags_;
	std::map<int, bool>				fuzzyMapping_;

	int								progressIndex_;
};

