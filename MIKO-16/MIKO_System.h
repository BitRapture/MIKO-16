#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <SDL.h>
#include <nfd.h>
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
		0x02050d, 0xd4f8fc,
		0x5c9bed, 0x0047ed,
		0x0400ed, 0x6e2be3,
		0xf02929, 0xf0311f,
		0xc1002b, 0x74000a,
		0x6a3918, 0xfa9813,
		0xf59c53, 0x00c37d,
		0x07865c, 0x004952
	};
	// 24-bit RGB colours, the MIKO colour palette

private:
	Uint8 miko_keys = 0, preserveKeys = 0;
	// 8-bit value that binds keypresses to be used in mem
	// MSB tells us if the same keys were pressed
	// preserveKeys is for the MSB

	Uint8 miko_keyControls = 0;
	char miko_keyHandler = 0;
	bool miko_keyDown = false, miko_keyUsed = false;
	// Preserves the last key pressed, it preserves most keys
	// mikoKeyUsed stops multiple uses of the same key, also an 8-bit value
	// mikoKeyControls preserves keys such as ctrl, shift and alt
	// Used for system purposes

	MIKO_Memory miko_mem;
	MIKO_Processor miko_cpu;
	// Instantiate the components

	SDL_Renderer* miko_render = nullptr;
	SDL_Event miko_event = {};
	// SDL variables, get reference of render
	// miko_event records user input

	SDL_Surface* miko_scfg = SDL_CreateRGBSurface(0, 256, 256, 24, 0, 0, 0, 0);
	SDL_Texture* miko_txfg = nullptr;
	SDL_Rect miko_display = { 0, 0, 256, 256 };
	// Sets up the rendering display surface

	SDL_Surface* miko_sctxt = SDL_LoadBMP("MIKO_Alphabet.bmp");
	// Load MIKO_Alphabet for UI usage

	enum class MIKO_State { MIKO_CONSOLE, MIKO_CART, MIKO_EDITOR, MIKO_IDE } miko_program = MIKO_State::MIKO_CONSOLE;
	// Enum contains all system states

	std::vector<std::string> miko_console_args, miko_console_commands;
	std::vector<std::vector<std::string>> miko_console_history;
	// Console arguments for commands

	nfdchar_t* miko_lastPath = nullptr;
	// The last file path used to find a .miko file

private:
	void printText(std::string, Uint8, Uint8, Uint8);
	// parses a string and prints to screen

	bool logArgs();
	// send user input to the args vector
	// returns true when user presses enter

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

