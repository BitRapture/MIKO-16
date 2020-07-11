#include "MIKO_System.h"

int main(int, char**)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		SDL_Log("[SDL2] Init Error: %s", SDL_GetError());
		return 400;
	}

	SDL_Window* miko_window = SDL_CreateWindow("MIKO-16", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 240, SDL_WINDOW_SHOWN);
	if (miko_window == nullptr)
	{
		SDL_Log("[SDL2] Window Creation Error: %s", SDL_GetError());
		SDL_Quit();
		return 401;
	}

	SDL_Renderer* miko_render = SDL_CreateRenderer(miko_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (miko_render == nullptr)
	{
		cleanup(miko_window);
		SDL_Log("[SDL2] Renderer Creation Error: %s", SDL_GetError());
		SDL_Quit();
		return 402;
	}

	SDL_Event miko_event;
	MIKO_System MIKO;	// Instanciate the system

	// Miko's game loop
	while (MIKO.miko_sys_running)
	{
		while (SDL_PollEvent(&miko_event))
		{
			switch (miko_event.type)
			{
			case SDL_QUIT:
				MIKO.miko_sys_running = false;
				break;
			case SDL_KEYDOWN:
				switch (miko_event.key.keysym.sym)
				{
				case SDLK_RIGHT:
					MIKO.miko_keys |= 0x1;
					break;
				case SDLK_LEFT:
					MIKO.miko_keys |= 0x2;
					break;
				case SDLK_UP:
					MIKO.miko_keys |= 0x4;
					break;
				case SDLK_DOWN:
					MIKO.miko_keys |= 0x8;
					break;
				case SDLK_z:
					MIKO.miko_keys |= 0x10;
					break;
				case SDLK_x:
					MIKO.miko_keys |= 0x20;
					break;
				case SDLK_ESCAPE:
					MIKO.miko_keys |= 0x40;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (miko_event.key.keysym.sym)
				{
				case SDLK_RIGHT:
					MIKO.miko_keys ^= 0x1;
					break;
				case SDLK_LEFT:
					MIKO.miko_keys ^= 0x2;
					break;
				case SDLK_UP:
					MIKO.miko_keys ^= 0x4;
					break;
				case SDLK_DOWN:
					MIKO.miko_keys ^= 0x8;
					break;
				case SDLK_z:
					MIKO.miko_keys ^= 0x10;
					break;
				case SDLK_x:
					MIKO.miko_keys ^= 0x20;
					break;
				case SDLK_ESCAPE:
					MIKO.miko_keys ^= 0x40;
					break;
				}
				break;
			}
		}

		
	}


	cleanup(miko_window, miko_render);
	SDL_Quit();
	return 200;
}