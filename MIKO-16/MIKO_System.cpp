#include "MIKO_System.h"

bool MIKO_System::initialise(SDL_Renderer* ren)
{
	miko_cpu.to_mem = &miko_mem;
	miko_render = ren;

	SDL_SetColorKey(miko_sctxt, SDL_TRUE, 0xffffff);

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
	SDL_SetColorKey(temp_surf, SDL_TRUE, 0x000000);
	SDL_Rect src_chr = { 0, 0, 16, 16 }, dst_chr = { x, y, 256, 256 };
	int dst_x = 0;

	for (unsigned i = 0; i < (unsigned)msg.length(); ++i)
	{
		++dst_x;
		if ((msg.at(i) - 0x61) < 0 || (msg.at(i) - 0x61) > 0x7A)
			switch (msg.at(i))
			{
			default:
				src_chr.x = -16;
			}
		else
			src_chr.x = (msg.at(i) - 0x61) * 16;
		SDL_BlitSurface(miko_sctxt, &src_chr, temp_surf, &dst_chr);
		SDL_BlitSurface(temp_surf, &dst_chr, miko_scfg, &dst_chr);
		dst_chr.x = x + (dst_x * 16);
	}

	SDL_FreeSurface(temp_surf);
}

bool MIKO_System::logArgs()
{
	if (miko_keyDown)
	{
		if (!miko_keyUsed)
		{
			if (miko_console_args.empty())
				miko_console_args.push_back("");
			miko_console_args.back() += miko_keyHandler;

			if (miko_keyHandler == ' ')
				miko_console_args.push_back("");

			miko_keyUsed = true;
		}
		if ((miko_keyControls & 0x1) == 0x1)
		{
			if (!miko_console_args.empty())
			{
				for (int i = miko_console_args.size() - 1; i > -1; --i)
				{
					if (miko_console_args[i] != "")
					{
						miko_console_args[i].pop_back();
						break;
					}
					miko_console_args.pop_back();
				}
			}
			miko_keyControls ^= 0x1;
		}
		if ((miko_keyControls & 0x20) == 0x20)
		{
			miko_keyControls ^= 0x20;
			return true;
		}
	}
	return false;
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
				miko_keyUsed = false;
				break;
			case SDLK_b:
				miko_keyHandler = 'b';
				miko_keyUsed = false;
				break;
			case SDLK_c:
				miko_keyHandler = 'c';
				miko_keyUsed = false;
				break;
			case SDLK_d:
				miko_keyHandler = 'd';
				miko_keyUsed = false;
				break;
			case SDLK_e:
				miko_keyHandler = 'e';
				miko_keyUsed = false;
				break;
			case SDLK_f:
				miko_keyHandler = 'f';
				miko_keyUsed = false;
				break;
			case SDLK_g:
				miko_keyHandler = 'g';
				miko_keyUsed = false;
				break;
			case SDLK_h:
				miko_keyHandler = 'h';
				miko_keyUsed = false;
				break;
			case SDLK_i:
				miko_keyHandler = 'i';
				miko_keyUsed = false;
				break;
			case SDLK_j:
				miko_keyHandler = 'j';
				miko_keyUsed = false;
				break;
			case SDLK_k:
				miko_keyHandler = 'k';
				miko_keyUsed = false;
				break;
			case SDLK_l:
				miko_keyHandler = 'l';
				miko_keyUsed = false;
				break;
			case SDLK_m:
				miko_keyHandler = 'm';
				miko_keyUsed = false;
				break;
			case SDLK_n:
				miko_keyHandler = 'n';
				miko_keyUsed = false;
				break;
			case SDLK_o:
				miko_keyHandler = 'o';
				miko_keyUsed = false;
				break;
			case SDLK_p:
				miko_keyHandler = 'p';
				miko_keyUsed = false;
				break;
			case SDLK_q:
				miko_keyHandler = 'q';
				miko_keyUsed = false;
				break;
			case SDLK_r:
				miko_keyHandler = 'r';
				miko_keyUsed = false;
				break;
			case SDLK_s:
				miko_keyHandler = 's';
				miko_keyUsed = false;
				break;
			case SDLK_t:
				miko_keyHandler = 't';
				miko_keyUsed = false;
				break;
			case SDLK_u:
				miko_keyHandler = 'u';
				miko_keyUsed = false;
				break;
			case SDLK_v:
				miko_keyHandler = 'v';
				miko_keyUsed = false;
				break;
			case SDLK_w:
				miko_keyHandler = 'w';
				miko_keyUsed = false;
				break;
			case SDLK_y:
				miko_keyHandler = 'y';
				miko_keyUsed = false;
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
				miko_keyUsed = false;
				break;
			case SDLK_x:
				miko_keys |= 0x20;
				miko_keyHandler = 'x';
				miko_keyUsed = false;
				break;
			case SDLK_ESCAPE:
				miko_keys |= 0x40;
				break;
			case SDLK_BACKSPACE:
				miko_keyControls |= 0x1;
				break;
			case SDLK_RSHIFT:
			case SDLK_LSHIFT:
				miko_keyControls |= 0x2;
				break;
			case SDLK_LALT:
			case SDLK_RALT:
				miko_keyControls |= 0x4;
				break;
			case SDLK_LCTRL:
			case SDLK_RCTRL:
				miko_keyControls |= 0x8;
				break;
			case SDLK_SPACE:
				miko_keyControls |= 0x10;
				miko_keyHandler = ' ';
				miko_keyUsed = false;
				break;
			case SDLK_RETURN:
				miko_keyControls |= 0x20;
				break;
			}
			miko_keyDown = true;
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

	SDL_FillRect(miko_scfg, &miko_display, miko_colours[0]);

	int p_x = 0, p_y = 0, p_c = 3;;
	for (std::string i : miko_console_args)
	{
		if (i == "run " || i == "run")
			p_c = 5;
		else
			p_c = 3;

		printText(i, p_c, p_x, 0);
		p_x += i.length() * 16;
	}

	SDL_Log("%i", miko_console_args.size());

	if (logArgs())
	{
		if (!miko_console_args.empty())
			miko_console_args.clear();
	}
}
