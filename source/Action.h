#pragma once

enum Action : unsigned int
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
