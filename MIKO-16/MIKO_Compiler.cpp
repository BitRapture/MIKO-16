#include "MIKO_Compiler.h"

struct Branch
{
	std::string name = "";
	unsigned long loc = 0;
};

MIKO_Compiler::MIKO_Compiler()
{
	islist.push_back("adc");
	islist.push_back("and");
	islist.push_back("asl");
	islist.push_back("bcc");
	islist.push_back("bcs");
	islist.push_back("beq");
	islist.push_back("bit");
	islist.push_back("bmi");
	islist.push_back("bne");
	islist.push_back("bpl");
	islist.push_back("bvc");
	islist.push_back("bvs");
	islist.push_back("clc");
	islist.push_back("cld");
	islist.push_back("clv");
	islist.push_back("cmp");
	islist.push_back("cpx");
	islist.push_back("cpy");
	islist.push_back("dec");
	islist.push_back("dex");
	islist.push_back("dey");
	islist.push_back("eor");
	islist.push_back("inc");
	islist.push_back("inx");
	islist.push_back("iny");
	islist.push_back("jmp");
	islist.push_back("jsr");
	islist.push_back("lda");
	islist.push_back("ldx");
	islist.push_back("ldy");
	islist.push_back("lsr");
	islist.push_back("nop");
	islist.push_back("ora");
	islist.push_back("pha");
	islist.push_back("php");
	islist.push_back("pla");
	islist.push_back("plp");
	islist.push_back("rol");
	islist.push_back("ror");
	islist.push_back("rts");
	islist.push_back("sbc");
	islist.push_back("sec");
	islist.push_back("sed");
	islist.push_back("sta");
	islist.push_back("stx");
	islist.push_back("sty");
	islist.push_back("tax");
	islist.push_back("tay");
	islist.push_back("tsx");
	islist.push_back("txa");
	islist.push_back("txs");
	islist.push_back("tya");
}

Uint8 address_modes(std::string cint)
{
	if (cint.length() > 3)
	{
		switch (cint.at(3))
		{
		case '(':
			if (cint.length() == 10 && cint.at(9) == ')')
			{
				if (cint.at(7) == ',')
				{
					// Indexed Indirect
					return 7;
				}
				else if (cint.at(4) == '$')
				{
					// Indirect
					return 6;
				}
				else
					return 0;
			}
			else if (cint.length() == 10 && cint.at(7) == ')' && cint.at(8) == ',')
			{
				// Indirect Indexed
				return 8;
			}
			else
				return 0;
			break;
		case '#':
			switch (cint.length())
			{
			case 7:
				// Immediate
				return 5;
				break;
			default:
				return 0;
			}
			break;
		case '$':
			switch (cint.length())
			{
			case 6:
				// Zero Page
				return 3;
				break;
			case 8:
				if (cint.at(6) == ',')
				{
					// Zero Page (X, Y)
					return 4;
				}
				else 
				{
					// Absolute
					return 1;
				}
				break;
			case 10:
				if (cint.at(8) == ',')
				{
					// Absolute (X, Y)
					return 2;
				}
				break;
			default:
				return 0;
			}
			break;
		default:
			return 0;
		}
	}

	return 0;
}

/*
	Modes ref sheet:
	None	0;
	Abs		1;	2B
	Abs XY	2;	2B
	ZerP	3;	1B
	ZerP XY	4;	1B
	Immed	5;	1B
	Indir	6;	2B
	IdxInd	7;	1B
	IndIdx	8;	1B
*/

bool abs_address(std::vector<Uint8>* raw, unsigned long* pc, std::string cint)
{
	if (cint.find_first_not_of("0123456789abcdef", 3) == std::string::npos)
	{
		std::string byte;
		raw->push_back(0x01);
		byte = cint[6];
		byte += cint[7];
		raw->push_back((Uint8)strtoul(byte.c_str(), NULL, 16));
		byte = cint[4];
		byte += cint[5];
		raw->push_back((Uint8)strtoul(byte.c_str(), NULL, 16));
		*pc += 3;
		return true;
	}
	else
		return false;
}

bool absxy_address(std::vector<Uint8>* raw, unsigned long* pc, std::string cint)
{
	std::string ad = cint;
	ad.pop_back();
	ad.pop_back();
	if (cint.at(9) == 'x' || cint.at(9) == 'y')
	{
		if (ad.find_first_not_of("0123456789abcdef", 3) == std::string::npos)
		{
			std::string byte;
			raw->push_back(0x02);
			byte = cint[6];
			byte += cint[7];
			raw->push_back((Uint8)strtoul(byte.c_str(), NULL, 16));
			byte = cint[4];
			byte += cint[5];
			raw->push_back((Uint8)strtoul(byte.c_str(), NULL, 16));
			*pc += 3;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool zerp_address(std::vector<Uint8>* raw, unsigned long* pc, std::string cint)
{
	if (cint.find_first_not_of("0123456789abcdef", 3) == std::string::npos)
	{
		std::string byte;
		raw->push_back(0x03);
		byte = cint[4];
		byte += cint[5];
		raw->push_back((Uint8)strtoul(byte.c_str(), NULL, 16));
		*pc += 2;
		return true;
	}
	else
		return false;
}

bool adc_inst(std::vector<Uint8>* raw, unsigned long* pc, std::string cint)
{
	raw->push_back(0x01);
	switch (address_modes(cint))
	{
	case 1:
		return abs_address(raw, pc, cint);
		break;
	case 2:
		return absxy_address(raw, pc, cint);
		break;
	case 3:
		return zerp_address(raw, pc, cint);
		break;
	case 4:
	case 5:
	case 7:
	case 8:
		break;
	default:
		return false;
	}
}

std::string MIKO_Compiler::compile(std::string path)
{
	std::vector<Branch> branches;
	std::vector<Uint8> raw;
	unsigned long pc = 0;
	std::string cint;
	char op = 0;

	bool checkline = false, opfound = false;

	std::ifstream infile(path, std::ifstream::binary | std::ifstream::ate);
	std::string outpath = path;
	outpath.pop_back();
	std::ofstream outfile(outpath.c_str(), std::ofstream::binary);
	infile.seekg(7);

	for (unsigned long i = 0; i < infile.tellg(); ++i)
	{
		infile.read(&op, 1);
		switch (op)
		{
		case '\n':
			checkline = true;
			break;
		case 0x0D:
		case ' ':
		case '	':
			break;
		default:
			cint += op;
		}
		if ((long long)i + 1 >= infile.tellg())
		{
			cint.pop_back();
			checkline = true;
		}
		
		if (checkline)
		{
			std::string possibleInst = "";
			if (cint.length() >= 3 && cint[cint.length() - 1] != ':')
			{
				possibleInst += cint[0];
				possibleInst += cint[1];
				possibleInst += cint[2];
			}
			auto it = std::find(islist.begin(), islist.end(), possibleInst);
			auto ist = std::distance(islist.begin(), it);

			// Instruction found
			if (it != islist.end())
			{
				bool funcLine;
				switch (ist)
				{
				case 0:
					funcLine = adc_inst(&raw, &pc, cint);
				default:
					funcLine = false;
					break;
				}

				if (!funcLine)
				{
					infile.close();
					return "err";
				}

				++pc;
			}
			// Add branch
			else if (cint.length() > 1 && cint[cint.length() - 1] == ':')
			{
				cint.pop_back();
				for (Branch x : branches)
				{
					if (x.name == cint)
					{
						infile.close();
						return "err";
						break;
					}
				}
				Branch newBranch = { cint, pc };
				branches.push_back(newBranch);
				raw.push_back(0);
				++pc;
			}
			// Error
			else if (cint.length() >= 1)
			{
				SDL_Log(cint.c_str());
				SDL_Log("At: %i", i);
				SDL_Log("Size: %i", cint.length());
				infile.close();
				return "err";
				break;
			}

			cint.clear();
			checkline = false;
		}
		op = 0;
	}

	infile.close();
	return "fine";
}