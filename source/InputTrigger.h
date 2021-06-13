// Instantiate an InputTrigger per motion input available to a character before a match begins
// InputTriggers will "listen" to inputs every update
//     timer_ is the buffer duration for the move
//     If an input on a given update matches the first required input for a motion, the object moves on to the next input in line
//	   timer_ decrements by 1 every update. If timer_ reaches 0, undo all progres
//	   If progress reaches end of motion before timer_ reachers 0, flip trigger_---InputTrigger::isTriggered() method informs that the motion input has triggered
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
	bool							trigger_;

	unsigned int					buffer_;
	unsigned int					timer_;
	std::vector<unsigned int>		motion_;
	std::vector<bool>				fuzzyInputFlags_;
	std::map<unsigned int, bool>	fuzzyMapping_;

	unsigned int					progressIndex_;
};

