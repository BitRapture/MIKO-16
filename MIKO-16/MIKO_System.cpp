#include "MIKO_System.h"

bool MIKO_System::initialise(SDL_Renderer* ren)
{
	miko_cpu.to_mem = &miko_mem;
	miko_render = ren;

	if (miko_mem.initial_check()) return true;

	return false;
}

MIKO_System::~MIKO_System()
{
	SDL_FreeSurface(miko_scfg);
	SDL_FreeSurface(miko_scbg);
	SDL_DestroyTexture(miko_txfg);
	SDL_DestroyTexture(miko_txbg);
}

void MIKO_System::updateKeys()
{
	Uint8 preserveKeys = miko_keys;
	while (SDL_PollEvent(&miko_event))
	{
		switch (miko_event.type)
		{
		case SDL_QUIT:
			miko_sys_running = false;
			break;
		case SDL_KEYDOWN:
			switch (miko_event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				miko_keys |= 0x1;
				break;
			case SDLK_LEFT:
				miko_keys |= 0x2;
				break;
			case SDLK_UP:
				miko_keys |= 0x4;
				break;
			case SDLK_DOWN:
				miko_keys |= 0x8;
				break;
			case SDLK_z:
				miko_keys |= 0x10;
				break;
			case SDLK_x:
				miko_keys |= 0x20;
				break;
			case SDLK_ESCAPE:
				miko_keys |= 0x40;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (miko_event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				miko_keys ^= 0x1;
				break;
			case SDLK_LEFT:
				miko_keys ^= 0x2;
				break;
			case SDLK_UP:
				miko_keys ^= 0x4;
				break;
			case SDLK_DOWN:
				miko_keys ^= 0x8;
				break;
			case SDLK_z:
				miko_keys ^= 0x10;
				break;
			case SDLK_x:
				miko_keys ^= 0x20;
				break;
			case SDLK_ESCAPE:
				miko_keys ^= 0x40;
				break;
			}
			break;
		}
	}
	if ((preserveKeys << 1) == (miko_keys << 1) && miko_keys != 0)
		miko_keys |= 0x80;
	else if ((preserveKeys & 0x80) == 0x80)
		miko_keys ^= 0x80;
}

void MIKO_System::updateSys()
{
	SDL_FillRect(miko_scfg, &miko_display, 0x000000);
	SDL_FillRect(miko_scbg, &miko_display, 0x000000);
	
	switch (miko_program)
	{
	case MIKO_State::MIKO_MENU:
		renderMenu();
		break;
	}

	miko_txbg = SDL_CreateTextureFromSurface(miko_render, miko_scbg);
	SDL_RenderCopy(miko_render, miko_txbg, NULL, &miko_display);
	miko_txfg = SDL_CreateTextureFromSurface(miko_render, miko_scfg);
	SDL_RenderCopy(miko_render, miko_txfg, NULL, &miko_display);
}

void MIKO_System::renderMenu()
{
	SDL_Rect Title = {
		b_x, b_y,
		20, 20
	};

	b_x += (((miko_keys & 0x1) == 1) - ((miko_keys & 0x2) == 2)) * 4;
	b_y += (((miko_keys & 0x8) == 8) - ((miko_keys & 0x4) == 4)) * 4;

	SDL_FillRect(miko_scfg, &Title, 0x0000ff);
}
