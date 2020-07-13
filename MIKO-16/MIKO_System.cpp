#include "MIKO_System.h"

bool MIKO_System::initialise(SDL_Renderer* ren)
{
	miko_cpu.to_mem = &miko_mem;
	miko_render = ren;

	SDL_SetColorKey(miko_sctxt, SDL_TRUE, 0x000000);

	if (miko_mem.initial_check()) return true;

	return false;
}

MIKO_System::~MIKO_System()
{
	SDL_FreeSurface(miko_scfg);
	SDL_FreeSurface(miko_sctxt);
	SDL_DestroyTexture(miko_txfg);
}

void MIKO_System::printText(std::string msg, Uint8 color, Uint8 x, Uint8 y)
{
	SDL_Surface* temp_surf = SDL_CreateRGBSurface(0, 256, 256, 24, 0, 0, 0, 0);
	SDL_FillRect(temp_surf, NULL, miko_colours[color]);
	
	SDL_Rect src_chr = { 0, 0, 16, 16 }, dst_chr = { x, y, 256, 256 };
	int dst_x = 0;

	for (unsigned i = 0; i < (unsigned)msg.length(); ++i)
	{
		src_chr.x = (msg.at(i) - 0x61) * 16;
		SDL_BlitSurface(miko_sctxt, &src_chr, miko_scfg, &dst_chr);
		//SDL_BlitSurface(temp_surf, &src_chr, miko_scfg, &dst_chr);
		dst_chr.x += (dst_x * 16);
		++dst_x;
	}

	SDL_FreeSurface(temp_surf);
}

void MIKO_System::updateKeys()
{

	if ((preserveKeys << 1) == (miko_keys << 1) && miko_keys != 0 && miko_keys != 0x80)
		miko_keys |= 0x80;
	else if ((preserveKeys & 0x80) == 0x80)
		miko_keys ^= 0x80;
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
			case SDLK_a:
				miko_keyHandler = 'a';
				break;
			case SDLK_b:
				miko_keyHandler = 'b';
				break;
			case SDLK_c:
				miko_keyHandler = 'c';
				break;
			case SDLK_d:
				miko_keyHandler = 'd';
				break;
			case SDLK_e:
				miko_keyHandler = 'e';
				break;
			case SDLK_f:
				miko_keyHandler = 'f';
				break;
			case SDLK_g:
				miko_keyHandler = 'g';
				break;
			case SDLK_h:
				miko_keyHandler = 'h';
				break;
			case SDLK_i:
				miko_keyHandler = 'i';
				break;
			case SDLK_j:
				miko_keyHandler = 'j';
				break;
			case SDLK_k:
				miko_keyHandler = 'k';
				break;
			case SDLK_l:
				miko_keyHandler = 'l';
				break;
			case SDLK_m:
				miko_keyHandler = 'm';
				break;
			case SDLK_n:
				miko_keyHandler = 'n';
				break;
			case SDLK_o:
				miko_keyHandler = 'o';
				break;
			case SDLK_p:
				miko_keyHandler = 'p';
				break;
			case SDLK_q:
				miko_keyHandler = 'q';
				break;
			case SDLK_r:
				miko_keyHandler = 'r';
				break;
			case SDLK_s:
				miko_keyHandler = 's';
				break;
			case SDLK_t:
				miko_keyHandler = 't';
				break;
			case SDLK_u:
				miko_keyHandler = 'u';
				break;
			case SDLK_v:
				miko_keyHandler = 'v';
				break;
			case SDLK_w:
				miko_keyHandler = 'w';
				break;
			case SDLK_y:
				miko_keyHandler = 'y';
				break;
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
				miko_keyHandler = 'z';
				break;
			case SDLK_x:
				miko_keys |= 0x20;
				miko_keyHandler = 'x';
				break;
			case SDLK_ESCAPE:
				miko_keys |= 0x40;
				break;
			}
			miko_keyDown = true;
			miko_keyUsed = false;
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
			miko_keyDown = false;
			break;
		}
	}
	preserveKeys = miko_keys;
}

void MIKO_System::updateSys()
{
	switch (miko_program)
	{
	case MIKO_State::MIKO_CONSOLE:
		updateConsole();
		break;

	default:
		SDL_FillRect(miko_scfg, &miko_display, miko_colours[0]);
	}

	miko_txfg = SDL_CreateTextureFromSurface(miko_render, miko_scfg);
	SDL_RenderCopy(miko_render, miko_txfg, NULL, &miko_display);
	SDL_DestroyTexture(miko_txfg);
}

void MIKO_System::updateConsole()
{
	if (miko_keyDown)
		if (!miko_keyUsed)
		{
			log += miko_keyHandler;
			printText(log, 5, 0, 0);
			miko_keyUsed = true;

			SDL_Log("%s", log.c_str());
		}

}
