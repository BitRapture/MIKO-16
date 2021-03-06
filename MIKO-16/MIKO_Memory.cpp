#include "MIKO_Memory.h"

MIKO_Memory::~MIKO_Memory()
{
	delete ram_memory, char_rom, prog_rom;
	ram_memory = nullptr;
	char_rom = nullptr;
	prog_rom = nullptr;
}

bool MIKO_Memory::initial_check()
{
	if (ram_memory == nullptr || char_rom == nullptr) return true;
	return false;
}