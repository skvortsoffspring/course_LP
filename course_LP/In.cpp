#include "stdafx.h"

namespace In
{

	IN getin(wchar_t nameFile[])
	{
		int positionArr = 0;
		bool literal = false;
		bool allowSpaces = false;
		bool specialSymbols = true;
		unsigned short positionErr = 1;

		std::fstream fin(nameFile, std::ios_base::in);

		if (fin.fail())
			throw ERROR_THROW(110)

		IN in;

		in.text = new unsigned char[IN_MAX_LEN_TEXT];

		if (!in.text)
			throw ERROR_THROW(1);

		for (unsigned char symbol; symbol = fin.get(); positionErr++)
		{
			if (fin.eof())
				break;
			if (symbol == ENDL)
			{
				in.lines++;
				in.text[positionArr] = ENDL;
				positionErr = 0;
			}
			switch (in.code[symbol])
			{
				case IN::F:
				{
					throw ERROR_THROW_IN(111, in.lines, positionErr);
				}
				case IN::I:
				{
					in.ignor++;
					in.size++;
					break;
				}
				case IN::T:
				{
					if (symbol == LITERAL) {								//
						if (literal) literal = false;
						else literal = true;								//
						allowSpaces = false;
					}
					else if (allowSpaces)									//
						in.text[positionArr++] = SPACE;						//

					in.text[positionArr++] = symbol;
					in.size++;

					allowSpaces = false;
					specialSymbols = false;
					break;
				}
				case IN::E:
				{
					in.text[positionArr++] = symbol;						//
					specialSymbols = true;									//
					allowSpaces = false;									//
					break;
				}
				case IN::S:
				{

					if (!allowSpaces && !specialSymbols)					//
						allowSpaces = true;								//
					else													//
						in.ignor++;											//

					if (literal)												//
						in.text[positionArr++] = symbol;					//

					in.size++;
					break;
				}
				case IN::L:
				{
					if (literal)											//
						in.text[positionArr++] = symbol;					//
					else													//
						throw ERROR_THROW_IN(111, in.lines, positionErr);	//
					break;
				}
				default:
				{
					if (literal)											//
						throw ERROR_THROW_IN(114, in.lines, positionErr);	//
					//if(unsigned char(in.code[symbol] - 1 ) != '|')
						in.text[positionArr++] = unsigned char(in.code[symbol]);
					allowSpaces = false;									//
					specialSymbols = true;									//
					break;
				}
			}


		}
		in.text[positionArr++] = LEX_RIGHTBRACE;
		if (literal)														//
			throw ERROR_THROW_IN(114, in.lines, positionErr);				//
		in.text[positionArr] = STR_ENDL;

		fin.close();
		return in;
	}
}
