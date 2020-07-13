#pragma once
#include <string>
#include <SDL.h>
#include "SDL Cleanup.h"

#include "MIKO_Memory.h"
#include "MIKO_Processor.h"

class MIKO_System
{
public:
	bool miko_sys_running = true;
	// Keeps the game loop running, false will exit the game loop

	Uint8 miko_sys_vers_major = 0x1, miko_sys_vers_minor = 0x0;
	// Miko System version number, Current 1.0;

private:
	Uint32 miko_colours[16] = {
		0x08040d, 0xcfd1ff,
		0x1b5fde, 0x8d36a2,
		0xdb446f, 0xdb0f0f,
		0xd34913, 0xe0a714,
		0x68e055, 0x0508e8,
		0x6026ea, 0xdb2a32,
		0xb4361d, 0xd88334,
		0x86a748, 0x34a784
	};
	// 24-bit RGB colours, the MIKO colour palette

private:
	Uint8 miko_keys = 0, preserveKeys = 0;
	// 8-bit value that binds keypresses
	// MSB tells us if the same keys were pressed
	// preserveKeys is for the MSB

	char miko_keyHandler = 0, miko_keyControls = 0;
	bool miko_keyDown = false, miko_keyUsed = false;
	// Preserves the last key pressed, it preserves most keys
	// mikoKeyUsed stops multiple uses of the same key
	// mikoKeyControls preserves keys such as ctrl, shift and alt
	// Used for system purposes

	MIKO_Memory miko_mem;
	MIKO_Processor miko_cpu;
	// Instantiate the components

	SDL_Renderer* miko_render = nullptr;
	SDL_Event miko_event = {};
	// SDL variables, get reference of render
	// miko_event records user input

	enum class MIKO_State { MIKO_CONSOLE, MIKO_CART, MIKO_EDITOR, MIKO_IDE } miko_program = MIKO_State::MIKO_CONSOLE;
	// Enum contains all system states

	SDL_Surface* miko_scfg = SDL_CreateRGBSurface(0, 256, 256, 24, 0, 0, 0, 0);
	SDL_Texture* miko_txfg;
	SDL_Rect miko_display = { 0, 0, 256, 256 };
	// Sets up the rendering display surface

	SDL_Surface* miko_sctxt = SDL_LoadBMP("MIKO_Alphabet.bmp");
	// Load MIKO_Alphabet for UI usage

private:
	int b_i = 0;
	std::string log;

	void printText(std::string, Uint8, Uint8, Uint8);
	// possible placeholder function
	// parses a string and prints to screen

	void updateConsole();
	// Render MIKO console

public:
	bool initialise(SDL_Renderer*);
	// Initialises connections
	// Returns true if failed to init

	void updateKeys();
	// Update keypress information

	void updateSys();
	// Update system

public:
	~MIKO_System();

};

