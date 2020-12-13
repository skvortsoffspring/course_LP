#include "stdafx.h"

namespace GTA
{
	enum REGISTERS { eax, ebx, ecx, edx};
	std::ofstream* file = new std::ofstream;
	void GeneratorASM(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		auto posfunc = new std::vector<int>;

		file->open(FILE, std::ios::out);
		if (file->fail())
			throw ERROR_THROW(115);
		PrintDataTime();

		*file << HEAD_ASM;
		*file << LIBRARY;
		*file << PROTO;
		PrintProtos(lextable, idtable, *posfunc);
		*file << CONST;
		*file << DATA;
		PrintLiterals(lextable, idtable);
		PrintVariables(lextable, idtable);
		PrintFunctions(lextable, idtable, *posfunc);

	}
	void PrintDataTime()
	{
		char buffer[48];
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 48, "%d-%m-%Y --%A-- %H:%M:%S ", timeinfo);
		*file << ";--" << buffer << "--\n";
	}
	void PrintLiterals(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		*file << COMMENTLITERAL;
		char buffer[32];
		int index = 0;
		for (int i = 0; i < lextable->size; i++)
		{
			if (lextable->table[i].lexema[0] == LEX_LITERAL && index < lextable->table[i].idxTI)
			{
				switch (idtable->table[lextable->table[i].idxTI].iddatatype)
				{
				case IT::LONG:
				{
					std::string temp = "literal";
					temp += UNDERLINE;
					temp += itoa(lextable->table[i].idxTI, buffer, 10);
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "DWORD";
					*file << idtable->table[lextable->table[i].idxTI].value.vint;
					*file << std::endl;
					break;
				}
				case IT::FLOAT:
				{
					std::string temp = "literal";
					temp += UNDERLINE;
					temp += itoa(lextable->table[i].idxTI, buffer, 10);
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "REAL4";
					*file << idtable->table[lextable->table[i].idxTI].value.vfloat;
					*file << std::endl;
					break;
				}
				case IT::STR:
				{
					std::string temp = "literal";
					temp += UNDERLINE;
					temp += itoa(lextable->table[i].idxTI, buffer, 10);
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "BYTE";
					*file << "\"";
					*file << idtable->table[lextable->table[i].idxTI].value.vstr->str;
					*file << "\"," << "str_end";
					*file << std::endl;
					break;
				}
				}
				index = lextable->table[i].idxTI;
			}
		}
	}
	void PrintProtos(LT::LexTable* lextable, IT::IdTable* idtable, std::vector<int>& posfunc)
	{
		*file << COMMENTPROTO;

		for (int i = 0; i < lextable->size; i++)
		{
			if (idtable->table[i].idtype == IT::F)
			{
				posfunc.push_back(i);
				if (lextable->table[idtable->table[i].idxfirstLE].lexema[0] == LEX_MAIN)
					continue;
				file->setf(std::ios::left);
				file->width(24);
				*file << idtable->table[i].id;
				*file << "PROTO : ";

				while (idtable->table[++i].idtype == IT::P) {
					switch (idtable->table[i].iddatatype)
					{
					case IT::CHAR:
					case IT::BOOL:
					{
						*file << BYTE;
						break;
					}
					case IT::LONG:
					case IT::STR:
					{
						*file << DWORD;
						break;
					}
					case IT::FLOAT:
					{
						*file << REAL4;
						break;
					}
					}
					if (idtable->table[i + 1].idtype == IT::P)
						*file << SPP;


				}
				*file << std::endl;
			}
		}

	}
	void PrintVariables(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		*file << COMMENTVAR;
		int index = 0;
		for (int i = 0; i < lextable->size; i++)
		{
			if (lextable->table[i].lexema[0] == LEX_ID &&
				index < lextable->table[i].idxTI)
			switch (idtable->table[lextable->table[i].idxTI].idtype)
			{
			case IT::V:
			{
				std::string temp;
				switch (idtable->table[lextable->table[i].idxTI].iddatatype)
				{
				case IT::LONG:
				{

					temp += idtable->table[lextable->table[i].idxTI].id;
					temp += UNDERLINE;
					temp += idtable->table[lextable->table[i].idxTI].prefix;
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "DWORD";
					*file << idtable->table[lextable->table[i].idxTI].value.vint;
					*file << std::endl;
					break;
				}
				case IT::CHAR:
				case IT::BOOL:
				{

					temp += idtable->table[lextable->table[i].idxTI].id;
					temp += UNDERLINE;
					temp += idtable->table[lextable->table[i].idxTI].prefix;
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "BYTE";
					switch (idtable->table[lextable->table[i].idxTI].iddatatype)
					{
					case IT::CHAR:
						*file << int(idtable->table[lextable->table[i].idxTI].value.vchar);
						break;
					case IT::BOOL:
						*file << idtable->table[lextable->table[i].idxTI].value.vbool;
						break;
					}
					*file << std::endl;

					break;
				}
				case IT::FLOAT:
				{
					temp += idtable->table[lextable->table[i].idxTI].id;
					temp += UNDERLINE;
					temp += idtable->table[lextable->table[i].idxTI].prefix;
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << REAL4;
					if (idtable->table[lextable->table[i].idxTI].value.vfloat == 0)
						*file << DEFREAL;
					else
						*file << idtable->table[lextable->table[i].idxTI].value.vfloat;

					*file << std::endl;
					break;
				}
				case IT::STR:
				{
					char buffer[4];
					temp += idtable->table[lextable->table[i].idxTI].id;
					temp += UNDERLINE;
					temp += idtable->table[lextable->table[i].idxTI].prefix;
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "DB";
					*file << TI_STR_MAXSIZE;
					*file << DUP0;
					*file << std::endl;
					break;
				}
				}
				break;

			}
			case IT::A:
			{
				std::string temp;
				switch (idtable->table[lextable->table[i].idxTI].iddatatype)
				{
				case IT::LONG:
				{
					temp += idtable->table[lextable->table[i].idxTI].id;
					temp += UNDERLINE;
					temp += idtable->table[lextable->table[i].idxTI].prefix;
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "DWORD";
					if (idtable->table[lextable->table[i + 2].idxTI].id[0]) {
						*file << idtable->table[lextable->table[i + 2].idxTI].id;
						*file << UNDERLINE;
						*file << idtable->table[lextable->table[i + 2].idxTI].prefix;
					}
					else
						*file << idtable->table[lextable->table[i + 2].idxTI].value.vint;
					*file << DUP0 << std::endl;
					break;
				}
				}
			}
			index = lextable->table[i].idxTI;
			}
			
		}
	}
	void PrintFunctions(LT::LexTable* lextable, IT::IdTable* idtable, std::vector<int>& posfunc)
	{
		*file << COMMENTCODE;
		*file << CODE;

		auto iter = posfunc.cbegin();
		int countbraces = 0;
		int poslenta = 0;															// print functions and parametre id and type
		int positionnFunc = 0;
		for (int i = 0, p = 0; i < posfunc.size(); i++)
		{
			p = posfunc[i];

#pragma region printID&Parametres
			if (idtable->table[p].idtype == IT::F)
			{
				positionnFunc = p;
				file->setf(std::ios::left);
				*file << idtable->table[p].id;
				*file << SPACE;
				*file << PROC;
				*file << SPACE;

				if (idtable->table[p + 1].idtype == IT::P)
					*file << COMMA;
				if (i == -1) *file << ENDL << START;

				while (idtable->table[++p].idtype == IT::P) {
					*file << idtable->table[p].id;
					*file << UNDERLINE;
					*file << idtable->table[p].prefix;
					*file << SFP;
					switch (idtable->table[p].iddatatype)
					{
					case IT::CHAR:
					case IT::BOOL:
					{
						*file << BYTE;
						break;
					}
					case IT::LONG:
					case IT::STR:
					{
						*file << DWORD;
						break;
					}
					case IT::FLOAT:
					{
						*file << REAL4;
						break;
					}
					}
					if (idtable->table[p + 1].idtype == IT::P)
						*file << COMMA << SPACE;
				}
				poslenta = idtable->table[--p].idxfirstLE;
#pragma region findstartfunction
				while (lextable->table[poslenta].lexema[0] != LEX_LEFTBRACE)
				{
					poslenta++;
				}
				*file << std::endl;
			}
#pragma endregion

#pragma endregion
#pragma region printbodyfuctions				
			do
			{
				if (lextable->table[poslenta].lexema[0] == LEX_LEFTBRACE)
					countbraces++;
				if (lextable->table[poslenta].lexema[0] == LEX_RIGHTBRACE)
					countbraces--;

				switch (lextable->table[poslenta].lexema[0])
				{
				case LEX_ID:
				{
					poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false);
					break;
				}
				}

				poslenta++;
			} while (countbraces);
			*file << std::endl;
			*file << RET;
			*file << idtable->table[positionnFunc].id << SPACE << ENDP;
			*file << std::endl;
		}
#pragma endregion
		* file << ENDMAIN;
		posfunc.clear();
	}
	int ProcessingDeclaration(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta, bool recursion)
	{
		if(!recursion)PrintChain(lextable, idtable, poslenta - 1);
		if (lextable->table[poslenta-1].lexema[0] == LEX_TYPE&&
			idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A
			) return poslenta + 1;
		int posID = lextable->table[poslenta++].idxTI;

		char ID[32];
		bool registers16 = true, registers32 = true, registersFPU = true;
		REGISTERS  reg32 = eax;
		int stack = 0;

		while (true) {
			std::string temp;
			switch (lextable->table[poslenta].lexema[0])
			{
			case LEX_ID:
			{
				switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
				{
				case IT::A:
				{
					int posarray = poslenta;
					switch (reg32) {
					case ebx:
					{
						*file << PUSH_EAX;
						break;
					}
					case ecx:
					{
						*file << PUSH_EAX << PUSH_EBX;
					}
					}
					*file << MOV_ECX_ADDR << idtable->table[lextable->table[poslenta].idxTI].id;	    // адресс массива asm
					*file << UNDERLINE;
					*file << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
					*file << PUSH_ECX;
					poslenta = ProcessingDeclaration(lextable, idtable, poslenta, true);					// рекурсия 
					*file << POP_ECX;
					*file << IMUL_EAX_TYPE << idtable->table[lextable->table[posarray].idxTI].id;			// вычеслить размер в байтах для смещения																// восстанавливаем регистр в котором содержится результат
					*file << UNDERLINE;
					*file << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
					*file << ADD_ECX_EAX;
					*file << MOV_EDX << PREFERRED_ECX;

					switch (reg32) {
					case ebx:
					{
						*file << POP_EAX;
						*file << MOV_EBX << "edx\n";
						reg32 = ecx;
						break;
					}
					case ecx:
					{
						*file << POP_EAX << POP_EBX;
						*file << PUSH_EDX;
						stack++;
					}
					default:
					{
						*file << MOV_EAX_EDX;
						reg32 = ebx;
					}
					}
					break;
				}
				case IT::V:
				{
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					{
						switch (reg32)
						{
						case eax:
						{
							*file << MOV_EAX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							*file << temp;
							*file << ENDL;
							reg32 = ebx;
							break;
						}
						case ebx:
						{
							*file << MOV_EBX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							*file << temp;
							*file << ENDL;
							reg32 = ecx;
							break;
						}
						case ecx:
						{
							*file << MOV_ECX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							*file << temp;
							*file << ENDL;
							*file << PUSH_ECX;
							stack++;
							break;
						}
						}

						break;
					}
					case IT::BOOL:
					case IT::CHAR:
					{
						switch (reg32)
						{
						case eax:
						{
							*file << MOV_AL;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							*file << temp;
							*file << ENDL;
							reg32 = ebx;
							break;
						}
						case ebx:
						{
							*file << MOV_BL;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							*file << temp;
							*file << ENDL;
							reg32 = ecx;
							break;
						}
						case ecx:
						{
							*file << MOV_CL;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							*file << temp;
							*file << ENDL;
							*file << PUSH_ECX;
							stack++;
							break;
						}
						}
						break;
					}
					case IT::FLOAT:
					{
						break;
					}
					}
					break;
				}
				}
				break;
			}
			case LEX_LITERAL:
			{
				switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
				{
				case IT::LONG:
				{
					switch (reg32)
					{
					case eax:
					{
						*file << MOV_EAX;
						temp = "literal";
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						*file << temp;
						*file << ENDL;
						reg32 = ebx;
						break;
					}
					case ebx:
					{
						*file << MOV_EBX;
						temp = "literal";
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						*file << temp;
						*file << ENDL;
						reg32 = ecx;
						break;
					}
					case ecx:
					{
						*file << MOV_ECX;
						temp = "literal";
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						*file << temp;
						*file << ENDL;
						*file << PUSH_ECX;
						stack++;
						break;
					}
					}
					break;
				}
				case IT::CHAR:
				case IT::BOOL:
				{
					switch (reg32)
					{
					case eax:
					{
						*file << MOV_AL;
						temp = "literal";
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						*file << temp;
						*file << ENDL;
						reg32 = ebx;
						break;
					}
					case ebx:
					{
						*file << MOV_BL;
						temp = "literal";
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						*file << temp;
						*file << ENDL;
						reg32 = ecx;
						break;
					}
					case ecx:
					{
						*file << MOV_CL;
						temp = "literal";
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						*file << temp;
						*file << ENDL;
						*file << PUSH_ECX;
						stack++;
						break;
					}
					}
					break;
				}
				case IT::FLOAT:
				{
					break;
				}
				}
				break;
			}
			case EXPRESSIONS:
				switch (lextable->table[poslenta].expression[0])
				{
				case ADD:
					switch (reg32)
					{
					case ebx:
					{
						*file << ADD_EAX_EBX;
						break;
					}
					case ecx:
					{
						if (stack > 1)
						{
							*file << POP_ECX << POP_EDX;
							*file << ADD_ECX_EDX;
							*file << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							*file << POP_ECX;
							*file << ADD_EBX_ECX;
							stack--;
							reg32 = ebx;
						}
						else {
							*file << ADD_EAX_EBX;
							reg32 = ebx;
						}
						break;
					}
					}
					break;

				case SUB:
					switch (reg32)
					{
					case ebx:
					{
						*file << SUB_EAX_EBX;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							*file << POP_ECX << POP_EDX;
							*file << SUB_ECX_EDX;
							*file << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							*file << POP_ECX;
							*file << SUB_EBX_ECX;
							stack--;
							reg32 = ebx;
						}
						else {
							*file << SUB_EAX_EBX;
							reg32 = ebx;
						}
						break;
					}
					}
					break;
				case DIV:
					switch (reg32)
					{
					case ebx:
					{

						*file << DIV_EAX_EBX;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							*file << POP_ECX << POP_EDX;
							*file << DIV_ECX_EDX;
							*file << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							*file << POP_ECX;
							*file << DIV_EBX_ECX;
							stack--;
							reg32 = ebx;
						}
						else {
							*file << DIV_EAX_EBX;
							reg32 = ebx;
						}
						break;
					}
					}
					break;
				case MUL:
					switch (reg32)
					{
					case ebx:
					{
						*file << MUL_EAX_EBX;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							*file << POP_ECX << POP_EDX;
							*file << MUL_ECX_EDX;
							*file << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							*file << POP_ECX;
							*file << MUL_EBX_ECX;
							stack--;
							reg32 = ebx;
						}
						else {
							*file << MUL_EAX_EBX;
							reg32 = ebx;
						}
						break;
					}
					}
					break;
				
				}
				break;

			case LEX_SEMICOLON:
			{
				switch (idtable->table[posID].iddatatype)
				{
				case IT::LONG:
				{
					*file << MOV;
					*file << idtable->table[posID].id;
					*file << UNDERLINE;
					*file << idtable->table[posID].prefix;
					*file << EAX << ENDL;
					break;
				}
				case IT::BOOL:
				case IT::CHAR:
				{
					*file << MOV;
					*file << idtable->table[posID].id;
					*file << UNDERLINE;
					*file << idtable->table[posID].prefix;
					*file << AL << ENDL;
					break;
				}
				case IT::FLOAT:
				{
					break;
				}


				}
				return poslenta - 1;
			}

			case LEX_SQUARE_RIGHT:
			{
				return poslenta;
			}
			}
			poslenta++;
			
		}
		return poslenta - 1;
	}
	void PrintChain(LT::LexTable* lextable, IT::IdTable* idtable, int pos)
	{
		int poslenta = pos;
		*file << SEMICOLON;
		*file << LEFTHESIS << lextable->table[poslenta].sn << RIGHTESIS;
		while (lextable->table[poslenta++].lexema[0] != SEMICOLON)
		{
			if (lextable->table[poslenta].idxTI == TI_NULLIDX)
			{
				if (lextable->table[poslenta].expression[0])
				{
					*file << lextable->table[poslenta].expression;
				}
				else
				{
					*file << lextable->table[poslenta].lexema[0];
				}
				*file << SPACE;
			}
			else
			{
				if (idtable->table[lextable->table[poslenta].idxTI].id[0])
				{
					*file << idtable->table[lextable->table[poslenta].idxTI].id;
					*file << SPACE;
				}
				else
				{
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					{
						*file << idtable->table[lextable->table[poslenta].idxTI].value.vint;
						break;
					}
					case IT::CHAR:
					{
						*file << idtable->table[lextable->table[poslenta].idxTI].value.vchar;
						break;
					}
					case IT::BOOL:
					{
						*file << idtable->table[lextable->table[poslenta].idxTI].value.vbool;
						break;
					}
					case IT::FLOAT:

					{
						*file << idtable->table[lextable->table[poslenta].idxTI].value.vfloat;
						break;
					}
					case IT::STR:
					{
						*file << idtable->table[lextable->table[poslenta].idxTI].value.vstr->str;
						break;
					}

					}
					*file << SPACE;
				}



			}
		}
		*file << ENDL;
	}
}

