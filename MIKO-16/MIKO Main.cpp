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

	SDL_Renderer* miko_render = SDL_CreateRenderer(miko_window, -1, SDL_RENDERER_ACCELERATED);
	if (miko_render == nullptr)
	{
		cleanup(miko_window);
		SDL_Log("[SDL2] Renderer Creation Error: %s", SDL_GetError());
		SDL_Quit();
		return 402;
	}

	MIKO_System MIKO;
	if (MIKO.initialise(miko_render))
	{
		cleanup(miko_window, miko_render);
		SDL_Log("[MIKO] Memory Allocation Error, failed to allocate memory");
		SDL_Quit();
		return 403;
	}

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	// Miko's game loop
	while (MIKO.miko_sys_running)
	{
		frameStart = SDL_GetTicks();

		SDL_RenderClear(miko_render);

		MIKO.updateKeys();
		MIKO.updateSys();

		SDL_RenderPresent(miko_render);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}

	cleanup(miko_window, miko_render);
	SDL_Quit();
	return 200;
}