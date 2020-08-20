#include "MIKO_System.h"

bool MIKO_System::initialise(SDL_Renderer* ren)
{
	miko_cpu.to_mem = &miko_mem;
	miko_render = ren;

	miko_console_commands.push_back("exit");
	miko_console_commands.push_back("run");
	miko_console_commands.push_back("make");
	miko_console_commands.push_back("compile");
	miko_console_commands.push_back("build");
	miko_console_commands.push_back("clear");
	miko_console_commands.push_back("clr");

	writeCliMsg("       miko", true);

	SDL_SetColorKey(miko_sctxt, SDL_TRUE, 0xffffff);

	discordSDK.updateRPC("Idling", "MIKO CLI", "miko_cart_logo", "No cartridge loaded");

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
				src_chr.x = -16;
		else
			src_chr.x = (msg.at(i) - 0x61) * 16;
		SDL_BlitSurface(miko_sctxt, &src_chr, temp_surf, &dst_chr);
		SDL_BlitSurface(temp_surf, &dst_chr, miko_scfg, &dst_chr);
		dst_chr.x = x + (dst_x * 14);
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

			if (miko_argSize <= 17)
			{
				if (miko_keyHandler == ' ')
					miko_console_args.push_back("");
				else
					miko_console_args.back() += miko_keyHandler;
				++miko_argSize;
			}

			reRender = true;
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
						if (miko_console_args.size() == 1 && miko_console_args[i] == "")
							miko_console_args.pop_back();
						break;
					}
					miko_console_args.pop_back();
					break;
				}
			}
			reRender = true;
			if (miko_argSize > 0)
				--miko_argSize;
			miko_keyControls ^= 0x1;
		}
		if ((miko_keyControls & 0x20) == 0x20)
		{
			miko_keyControls ^= 0x20;
			miko_console_history.push_back(miko_console_args);
			miko_argSize = 0;
			reRender = true;
			return true;
		}
	}
	return false;
}

int MIKO_System::retArgCol(std::string txt, bool history = false)
{
	auto it = std::find(miko_console_commands.begin(), miko_console_commands.end(), txt);
	auto command = std::distance(miko_console_commands.begin(), it);

	if (history)
	{
		if (txt == "err")
			return 8;
		if (txt.length() > 1 && txt.at(0) == '`')
			return 13;
	}

	if (it != miko_console_commands.end())
	{
		switch (command)
		{
			// Exit
		case 0:
			return 8;
			break;
			// Run
			// Make/compile/build
			// Clear/clr
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			return 14;
			break;

		default:
			return 1;
		}
	}
	else
		if (txt.length() == 1)
		{
			switch (txt.at(0))
			{
			case 'r':
			case 'o':
				return 2;
				break;

			default:
				return 1;
			}
		}
		else
		return 1;
}

void MIKO_System::writeCliMsg(std::string msg, bool psh = false)
{
	miko_console_message.clear();
	std::string hold;
	msg.push_back(' ');
	for (char i : msg)
	{
		if (i == ' ')
		{
			miko_console_message.push_back(hold);
			hold.clear();
		}
		else
			hold.push_back(i);
	}
	if (psh)
	{
		miko_console_history.push_back(miko_console_message);
		reRender = true;
	}
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
				miko_keyHandler = 0x20;
				miko_keyUsed = false;
				break;
			case SDLK_RETURN:
				miko_keyControls |= 0x20;
				break;
			}
			if (miko_event.key.keysym.sym >= 97 && miko_event.key.keysym.sym <= 122)
			{
				miko_keyHandler = miko_event.key.keysym.sym;
				miko_keyUsed = false;
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
	case MIKO_State::MIKO_COMPILE:
		updateCompiler();
		break;
	default:
		SDL_FillRect(miko_scfg, &miko_display, miko_colours[0]);
	}

	discordSDK.updateSDK();

	miko_txfg = SDL_CreateTextureFromSurface(miko_render, miko_scfg);
	SDL_RenderCopy(miko_render, miko_txfg, NULL, &miko_display);
	SDL_DestroyTexture(miko_txfg);
}

bool checkPath(std::string path, bool type)
{
	std::ifstream file(path, std::ifstream::binary);
	char buffer[7];
	file.read(buffer, 7);
	if (type)
	{
		if (buffer[0] == 'm' && buffer[1] == 'i' && buffer[2] == 'k' && buffer[3] == 'o' && buffer[4] == ' ')
		{
			return true;
		}
	}
	else
		if (buffer[0] == 'm' && buffer[1] == 'i' && buffer[2] == 'k' && buffer[3] == 'o' && buffer[4] == 'c' && buffer[5] == 0x0D && buffer[6] == 0x0A)
		{
			return true;
		}
	file.close();
	return false;
}

void MIKO_System::updateConsole()
{
	auto args = miko_console_args.size();
	auto it = (args > 0) ? std::find(miko_console_commands.begin(), miko_console_commands.end(), miko_console_args[0]) : miko_console_commands.end();
	auto command = std::distance(miko_console_commands.begin(), it);

	// Handle commands
	if (logArgs() && !miko_console_args.empty())
	{
		if (it != miko_console_commands.end())
		{
			switch (command)
			{
				// Exit
			case 0:
				if (args == 1)
					miko_sys_running = false;
				else
					writeCliMsg("err `b", true);
				break;

				// Run
			case 1:
				if (args == 2 && miko_console_args[1].length() == 1)
				{
					switch (miko_console_args[1].at(0))
					{
						// Open recent file
					case 'r':
						if (miko_lastPath != nullptr)
						{
							if (checkPath(miko_lastPath, true))
							{
								miko_program = MIKO_State::MIKO_CART;
							}
							else
							{
								miko_lastPath = nullptr;
								writeCliMsg("err `c", true);
							}
						}
						break;

						// Open file explorer
					case 'o':
						if (NFD_OpenDialog("miko", NULL, &miko_lastPath) == NFD_OKAY)
						{
							if (checkPath(miko_lastPath, true))
							{
								miko_program = MIKO_State::MIKO_CART;
							}
							else
							{
								miko_lastPath = nullptr;
								writeCliMsg("err `c", true);
							}
						}
						else
						{
							miko_lastPath = nullptr;
							writeCliMsg("err `c", true);
						}
						break;

					default:
						writeCliMsg("err `b", true);
					}
				}
				else
					writeCliMsg("err `b", true);
				break;
			
				// Make/compile/build
			case 2:
			case 3:
			case 4:
				if (args == 2 && miko_console_args[1].length() == 1)
				{
					switch (miko_console_args[1].at(0))
					{
						// Open recent file
					case 'r':
						if (miko_lastPath != nullptr)
						{
							if (checkPath(miko_lastPath, false))
							{
								miko_program = MIKO_State::MIKO_COMPILE;
							}
							else
							{
								miko_lastPath = nullptr;
								writeCliMsg("err `c", true);
							}
						}
						break;

						// Open file explorer
					case 'o':
						if (NFD_OpenDialog("mikoC", NULL, &miko_lastPath) == NFD_OKAY)
						{
							if (checkPath(miko_lastPath, false))
							{
								miko_program = MIKO_State::MIKO_COMPILE;
							}
							else
							{
								miko_lastPath = nullptr;
								writeCliMsg("err `c", true);
							}
						}
						else
						{
							miko_lastPath = nullptr;
							writeCliMsg("err `c", true);
						}
						break;

					default:
						writeCliMsg("err `b", true);
					}
				}
				else
					writeCliMsg("err `b", true);
				break;

				// Clear/clr
			case 5:
			case 6:
				if (args == 1)
				{
					miko_console_history.clear();
					writeCliMsg("       miko", true);
				}
				else
					writeCliMsg("err `b", true);
				break;
			}
		}
		else
		{
			if (args > 0)
				writeCliMsg("err `a", true);
		}
		miko_console_args.clear();
	}

	// Control history size
	if (miko_console_history.size() > 15)
	{
		std::reverse(miko_console_history.begin(), miko_console_history.end());
		while (miko_console_history.size() > 15)
		{
			miko_console_history.pop_back();
		}
		std::reverse(miko_console_history.begin(), miko_console_history.end());
	}

	// Draw text and command formatting 
	if (reRender)
	{
		SDL_FillRect(miko_scfg, &miko_display, miko_colours[0]);

		int p_y, p_x = 0, ph_x = 0;
		for (p_y = 0; p_y < (int)miko_console_history.size() + 1; ++p_y)
		{
			ph_x = 0;
			if (p_y < (int)miko_console_history.size())
				for (std::string txt : miko_console_history[p_y])
				{
					printText(txt, retArgCol(txt, true), ph_x, p_y * 16);
					ph_x += (txt.length() * 14) + 14;
				}
			else
				for (std::string txt : miko_console_args)
				{
					printText(txt, retArgCol(txt), p_x, p_y * 16);
					p_x += (txt.length() * 14) + 14;
				}
		}
		reRender = false;
	}
}

void MIKO_System::updateCompiler()
{
	writeCliMsg(miko_comp.compile(miko_lastPath), true);

	miko_program = MIKO_State::MIKO_CONSOLE;
}