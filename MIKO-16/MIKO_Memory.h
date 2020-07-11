#pragma once
#include <SDL.h>

#include "MIKO_System.h"

class MIKO_Memory
{
private:
	Uint8* ram_memory = new Uint8[65536];
	// Allocate 64K of RAM for MIKO usage

	Uint8* prog_rom = nullptr, * char_rom = new Uint8[98304];
	// Initialise program ROM (unknown size) and character ROM (96K = 128 16*16 sprites)	

public:


public:
	~MIKO_Memory();
};
