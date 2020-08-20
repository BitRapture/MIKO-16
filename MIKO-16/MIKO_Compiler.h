#pragma once
#include <SDL.h>
#include <vector>
#include <fstream>
#include <string>

class MIKO_Compiler
{
	std::vector<std::string> islist;

public:
	std::string compile(std::string);
	// Compile a mikoC file 

public:
	MIKO_Compiler();
};

