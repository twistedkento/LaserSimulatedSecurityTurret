#pragma once
#include "filesystem.h"
class python
{
public:
	void run(std::string code);
	
	std::string  init = filesystem::open("turret_connect.py");
	std::string  deInit = filesystem::open("turret_disconnect.py");
	
	python();
	~python();
};

