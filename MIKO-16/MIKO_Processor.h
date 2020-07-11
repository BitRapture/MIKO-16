#pragma once
#include <SDL.h>

#include "MIKO_System.h"

class MIKO_Processor
{
private:
	Uint8 reg_a = 0, reg_x = 0, reg_y = 0, reg_sp = 0, reg_status = 0;
	// Initialise the CPU registers

	long long int reg_pc = 0;
	// Initialise the CPU program counter (64 bit) a.k.a Instruction Pointer

public:

};

