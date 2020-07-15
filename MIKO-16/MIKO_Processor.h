#pragma once
#include <SDL.h>

#include "MIKO_Memory.h"

class MIKO_Processor
{
private:
	Uint16 reg_a = 0, reg_x = 0, reg_y = 0, reg_sp = 0xff, reg_status = 0;
	// Initialise the CPU registers

	long long unsigned reg_pc = 0;
	// Initialise the CPU program counter (64 bit) a.k.a Instruction Pointer

public:
	MIKO_Memory* to_mem = nullptr;
	// Lets the CPU access Memory

};

