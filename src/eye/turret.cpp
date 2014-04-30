#include "turret.h"
#include <string>

std::string turret::turn_left = filesystem::open("turn_left.py");
std::string turret::turn_right = filesystem::open("turn_right.py");
std::string turret::turn_up = filesystem::open("turn_up.py");
std::string turret::turn_down = filesystem::open("turn_down.py");

python turret::pythonclient;

#include <iostream>


void turret::TurnLeft(){
	pythonclient.run(turn_left);
}
void turret::TurnRight(){
	pythonclient.run(turn_right);
}
void turret::TurnUp(){
	pythonclient.run(turn_up);
}
void turret::TurnDown(){
	pythonclient.run(turn_down);
}