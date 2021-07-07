#pragma once

namespace constants
{
	// Window resolution
	const int WINDOW_WIDTH  = 1280;
	const int WINDOW_HEIGHT = 720;

	//const int WINDOW_WIDTH  = 854;
	//const int WINDOW_HEIGHT = 480;

	// Game logic tick duration (in milliseconds)
	const float TICK_DURATION = 1000.f / 60.f;

	const float METER_MAX = 100.f;

	const unsigned int MAX_INPUT_BUFFER	= 60;
	const unsigned int INPUT_BUFFER_236			= 15; // buffer window length for registering a 236 motion
	const unsigned int INPUT_BUFFER_214			= 15; 
	const unsigned int INPUT_BUFFER_421			= 15; 
	const unsigned int INPUT_BUFFER_HCF			= 30; 
	const unsigned int INPUT_BUFFER_HCB			= 30; 
	const unsigned int INPUT_BUFFER_22			= 15; 

}
