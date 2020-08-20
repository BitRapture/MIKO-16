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

unsigned address_modes(std::string cint)
{
	if (cint.length() > 3)
	{
		switch (cint.at(3))
		{
		case '#':

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

bool adc_inst(std::vector<unsigned>* raw, unsigned long* pc, std::string cint)
{
	if (address_modes(cint) != 0)
	{

	}

	return false;
}

std::string MIKO_Compiler::compile(std::string path)
{
	std::vector<Branch> branches;
	std::vector<unsigned> raw;
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
				switch (ist)
				{
				case 0:
					adc_inst(&raw, &pc, cint);
				default:
					break;
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