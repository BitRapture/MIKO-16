#pragma once
#include <SDL.h>

class MIKO_Memory
{
private:
	Uint16* ram_memory = new Uint16[65536];
	// Allocate 64K of RAM for MIKO usage

	Uint16* prog_rom = nullptr, * char_rom = new Uint16[32768];
	// Initialise program ROM (unknown size) and character ROM (96K = 128 16*16 sprites)	

public:
	bool initial_check();
	// Checks if ram_memory and char_memory have allocated successfully
	// Returns true if memory failed to allocate

public:
	~MIKO_Memory();
};
