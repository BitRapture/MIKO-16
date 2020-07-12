#pragma once
#include <SDL.h>
#include "SDL Cleanup.h"

#include "MIKO_Memory.h"
#include "MIKO_Processor.h"

#include <iostream>
#include <bitset>

class MIKO_System
{
public:
	bool miko_sys_running = true;
	// Keeps the game loop running, false will exit the game loop


private:
	Uint8 miko_keys = 0, preserveKeys = 0;	
	// 8-bit value that binds keypresses
	// MSB tells us if the same keys were pressed

	Uint8 miko_colours[48] = {
		0x16, 0x17, 0x1a,
		0x7f, 0x6, 0x22,
		0xd6, 0x24, 0x11,
		0xff, 0x84, 0x26,
		0xff, 0xd1, 0,
		0xfa, 0xfd, 0xff,
		0xff, 0x80, 0xa4,
		0xff, 0x26, 0x74,
		0x94, 0x21, 0x6a,
		0x43, 0, 0x67,
		0x23, 0x49, 0x75,
		0x68, 0xae, 0xd4,
		0xbf, 0xff, 0x3c,
		0x10, 0xd2, 0x75,
		0, 0x78, 0x99,
		0, 0x28, 0x59
	};
	// 24-bit RGB colours
	// currently a placeholder palette by https://lospec.com/palette-list/bubblegum-16

private:
	MIKO_Memory miko_mem;
	MIKO_Processor miko_cpu;
	// Instanciate the components

	SDL_Renderer* miko_render = nullptr;
	SDL_Event miko_event = {};
	// SDL variables, get reference of render
	// miko_event records user input

	enum class MIKO_State { MIKO_MENU, MIKO_CART, MIKO_EDITOR, MIKO_IDE } miko_program = MIKO_State::MIKO_MENU;
	// Enum contains all system states

	SDL_Surface* miko_scfg = SDL_CreateRGBSurface(0, 256, 240, 24, 0, 0, 0, 0);
	SDL_Texture* miko_txfg;
	SDL_Rect miko_display = { 0, 0, 256, 240 };
	// Creates the screens

private:
	int b_x = 0, b_y = 0;

	void renderMenu();
	// Render MIKO menu

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

