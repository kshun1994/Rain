#include <rnpch.h>
#include <Input.h>


int down[]		= { 1, 2, 3 };
int up[]		= { 7, 8, 9 };
int forward[]	= { 3, 6, 9 };
int back[]		= { 1, 4, 7 };

int numpadToCharInput(char numpadInput)
{
	int charInput = 0;
	if (isdigit(numpadInput))
	{
		if (std::find(std::begin(down), std::end(down), numpadInput - '0') != std::end(down))			// Downward directions
		{
			charInput |= CharInput::CharDown;
		}
		if (std::find(std::begin(up), std::end(up), numpadInput - '0') != std::end(up))					// Upward directions
		{
			charInput |= CharInput::CharUp;
		}
		if (std::find(std::begin(forward), std::end(forward), numpadInput - '0') != std::end(forward))	// Forward directions
		{
			charInput |= CharInput::CharForward;
		}
		if (std::find(std::begin(back), std::end(back), numpadInput - '0') != std::end(back))			// Backward directions
		{
			charInput |= CharInput::CharBack;
		}
	}
	else if (isalpha(numpadInput))
	{
		switch (numpadInput)
		{
			case 'A':
				charInput |= CharInput::CharA;
				break;
			case 'B':
				charInput |= CharInput::CharB;
				break;
			case 'C':
				charInput |= CharInput::CharC;
				break;
			case 'D':
				charInput |= CharInput::CharD;
				break;
			case 'X': // 'X' will specify for the previous button in an input to be released
				charInput |= CharInput::CharRelease;
				break;
			default:
				break;
		}
	}
	return charInput;
};

