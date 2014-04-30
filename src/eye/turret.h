#pragma once
#include "python.h"

namespace turret
{
	extern std::string  turn_left;
	extern std::string  turn_right;
	extern std::string  turn_up;
	extern std::string turn_down;
	
	extern python pythonclient;

	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();
};

