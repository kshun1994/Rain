#pragma once

enum Input
{
	Left			= 1 << 0,
	Right			= 1 << 1,
	Up				= 1 << 2,
	Down			= 1 << 3,
	A				= 1 << 4,
	B				= 1 << 5,
	C				= 1 << 6,
	D				= 1 << 7,
	Start			= 1 << 8,
	Select			= 1 << 9,
};

enum CharInput
{
	CharForward			= 1 << 0,
	CharBack			= 1 << 1,
	CharUp				= 1 << 2,
	CharDown			= 1 << 3,
	CharA				= 1 << 4,
	CharB				= 1 << 5,
	CharC				= 1 << 6,
	CharD				= 1 << 7,
	CharStart			= 1 << 8,
	CharSelect			= 1 << 9,
	CharRelease			= 1 << 10,
};

int numpadToCharInput(char numpadInput);
