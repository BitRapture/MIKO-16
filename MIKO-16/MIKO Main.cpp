#include <iostream>
#include <SDL.h>

int main(int, char**)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		std::cout << "[SDL2] Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* miko_window = SDL_CreateWindow("MIKO-16", 100, 100, 256, 240, SDL_WINDOW_SHOWN);
	if (miko_window == nullptr)
	{
		std::cout << "[SDL2] Window Creation Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* miko_render = SDL_CreateRenderer(miko_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (miko_render == nullptr)
	{
		SDL_DestroyWindow(miko_window);
		std::cout << "[SDL2] Renderer Creation Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}


	return 1;
}