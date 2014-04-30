#include <sstream>
#include <fstream>
#include <iostream>
namespace filesystem{
	std::string open(char* fname){
		std::ifstream t(fname);
		std::stringstream buffer;
		buffer << t.rdbuf();
		return buffer.str();
	}
}