#include "stdafx.h"

namespace GTA
{
	bool overflow_check = false;
	int CountCondition = 0;
	int ifelse = 0;
	enum REGISTERS { eax, ebx, ecx, edx};
	std::ofstream* file = new std::ofstream;
	void GeneratorASM(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		auto posfunc = new std::vector<int>;

		file->open("..\\SIA-2020\\source.asm", std::ios::out);
		if (file->fail())
			throw ERROR_THROW(115);
		PrintDataTime();

		FILE << HEAD_ASM;
		FILE << LIBRARY;
		FILE << PROTO;
		PrintProtos(lextable, idtable, *posfunc);
		FILE << CONST;
		PrintConst(lextable, idtable);
		FILE << DATA;
		FILE << SYSTEMERRORS;
		PrintLiterals(lextable, idtable);
		PrintVariables(lextable, idtable);
		FILE << COMMENTCODE;
		FILE << CODE;
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
		FILE << ";--" << buffer << "--\n";
	}
	void PrintLiterals(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		FILE << COMMENTLITERAL;
		char buffer[32];
		int index = 0;
		for (int i = 0; i < lextable->size; i++)
		{
			//if (idtable->table[lextable->table[i].idxTI].idtype == IT::A)	/////?
				//notArray = false;											/////?
			//if (lextable->table[i].lexema[0] == LEX_SEMICOLON && !notArray) /////?
			//	notArray = true;

			if (lextable->table[i].lexema[0] == LEX_LITERAL && index < lextable->table[i].idxTI)
			{
				switch (idtable->table[lextable->table[i].idxTI].iddatatype)
				{
				case IT::LONG:
				{
					std::string temp = LITERALS ;
					temp += UNDERLINE;
					temp += itoa(lextable->table[i].idxTI, buffer, 10);
					file->setf(std::ios::left);
					file->width(24);
					FILE << temp;
					file->width(10);
					FILE << DWORD;
					FILE << idtable->table[lextable->table[i].idxTI].value.vint;
					FILE << std::endl;
					break;
				}
				case IT::FLOAT:
				{
					std::string temp = LITERALS ;
					temp += UNDERLINE;
					temp += itoa(lextable->table[i].idxTI, buffer, 10);
					file->setf(std::ios::left);
					file->width(24);
					FILE << temp;
					file->width(10);
					FILE << REAL4;
					FILE << idtable->table[lextable->table[i].idxTI].value.vfloat;
					FILE << std::endl;
					break;
				}
				case IT::STR:
				{
					std::string temp = LITERALS;
					temp += UNDERLINE;
					temp += itoa(lextable->table[i].idxTI, buffer, 10);
					file->setf(std::ios::left);
					file->width(24);
					FILE << temp;
					file->width(10);
					FILE << BYTE;
					FILE << "\"";
					FILE << idtable->table[lextable->table[i].idxTI].value.vstr->str;
					FILE << "\"," << STR_END;
					break;
				}
				}
				index = lextable->table[i].idxTI;
			}
		}
	}
	void PrintProtos(LT::LexTable* lextable, IT::IdTable* idtable, std::vector<int>& posfunc)
	{
		FILE << COMMENTPROTO;

		for (int i = 0; i < lextable->size; i++)
		{
			if (idtable->table[i].idtype == IT::F)
			{
				posfunc.push_back(i);
				if (lextable->table[idtable->table[i].idxfirstLE].lexema[0] == LEX_MAIN)
					continue;
				file->setf(std::ios::left);
				file->width(24);
				FILE << idtable->table[i].id;
				FILE << "PROTO : ";
				i++;
				while (idtable->table[i].idtype == IT::P || idtable->table[i].idtype == IT::PA) {
					switch (idtable->table[i].iddatatype)
					{
					case IT::CHAR:
					case IT::BOOL:
					{
						FILE << BYTE;
						break;
					}
					case IT::LONG:
					case IT::STR:
					{
						FILE << DWORD;
						break;
					}
					case IT::FLOAT:
					{
						FILE << REAL4;
						break;
					}
					}
					if (idtable->table[i + 1].idtype == IT::P || idtable->table[i+1].idtype == IT::PA)
						FILE << SPP;

					i++;
				}
				FILE << std::endl;
			}
		}

	}
	void PrintConst(LT::LexTable* lextable, IT::IdTable* idtable) 
	{
		FILE << USERCONST;
		for (int i = 0; i < idtable->current_size; i++)
		{
			if (idtable->table[i].idtype == IT::C) {
				std::string temp;
				temp += idtable->table[i].id;
				temp += UNDERLINE;
				temp += idtable->table[i].prefix;
				file->setf(std::ios::left);
				file->width(24);
				FILE << temp;
				file->width(10);
				if (lextable->table[idtable->table[i].idxfirstLE + 1].lexema[0] == LEX_ASSIGN)
				{
					switch (idtable->table[i].iddatatype)
					{
					case IT::LONG:
						FILE << EQU << idtable->table[i + 1].value.vint;
						break;
					case IT::CHAR:
					case IT::BOOL:
						FILE << EQU << idtable->table[i + 1].value.vchar;
						break;
					case IT::STR:
						FILE << EQU << idtable->table[i + 1].value.vstr->str;
					}
					idtable->table[i].iddatatype = IT::NUL;
				}
				else
					throw ERROR_THROW_LINE(138, lextable->table[idtable->table[i].idxfirstLE].sn);
				FILE << ENDL;
				
			}
		}
		FILE << ENDL;
	}
	void PrintVariables(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		FILE << COMMENTVAR;
		int index = 0;
		for (int i = 0; i < lextable->size; i++)
		{
			if (
				index < lextable->table[i].idxTI)
			{
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
						FILE << temp;
						file->width(10);
						FILE << "DWORD";
						FILE << idtable->table[lextable->table[i].idxTI].value.vint;
						FILE << std::endl;
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
						FILE << temp;
						file->width(10);
						FILE << "BYTE";
						switch (idtable->table[lextable->table[i].idxTI].iddatatype)
						{
						case IT::CHAR:
							FILE << int(idtable->table[lextable->table[i].idxTI].value.vchar);
							break;
						case IT::BOOL:
							FILE << idtable->table[lextable->table[i].idxTI].value.vbool;
							break;
						}
						FILE << std::endl;

						break;
					}
					case IT::FLOAT:
					{
						temp += idtable->table[lextable->table[i].idxTI].id;
						temp += UNDERLINE;
						temp += idtable->table[lextable->table[i].idxTI].prefix;
						file->setf(std::ios::left);
						file->width(24);
						FILE << temp;
						file->width(10);
						FILE << REAL4;
						if (idtable->table[lextable->table[i].idxTI].value.vfloat == 0)
							FILE << DEFREAL;
						else
							FILE << idtable->table[lextable->table[i].idxTI].value.vfloat;

						FILE << std::endl;
						break;
					}
					case IT::STR:
					{
						temp += idtable->table[lextable->table[i].idxTI].id;
						temp += UNDERLINE;
						temp += idtable->table[lextable->table[i].idxTI].prefix;
						file->setf(std::ios::left);
						file->width(24);
						FILE << temp;
						file->width(10);
						FILE << BYTE;
						FILE << TI_STR_MAXSIZE;
						FILE << DUP0;
						FILE << std::endl;
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
						FILE << temp;
						file->width(10);
						FILE << DWORD;
						if (idtable->table[lextable->table[i + 2].idxTI].id[0]) {
							FILE << idtable->table[lextable->table[i + 2].idxTI].id;
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[i + 2].idxTI].prefix;
						}
						else
							FILE << idtable->table[lextable->table[i + 2].idxTI].value.vint;
						FILE << DUP0 << std::endl;
						break;
					}
					case IT::BOOL:
					case IT::CHAR:
					{
						temp += idtable->table[lextable->table[i].idxTI].id;
						temp += UNDERLINE;
						temp += idtable->table[lextable->table[i].idxTI].prefix;
						file->setf(std::ios::left);
						file->width(24);
						FILE << temp;
						file->width(10);
						FILE << BYTE;
						if (idtable->table[lextable->table[i + 2].idxTI].id[0]) {
							FILE << idtable->table[lextable->table[i + 2].idxTI].id;
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[i + 2].idxTI].prefix;
						}
						else
							FILE << idtable->table[lextable->table[i + 2].idxTI].value.vint;
						FILE << DUP0 << std::endl;
						break;
					}
					}
				}
			}
			index = lextable->table[i].idxTI;
			}
			
		}
	}
	void PrintFunctions(LT::LexTable* lextable, IT::IdTable* idtable, std::vector<int>& posfunc)
	{
		auto iter = posfunc.cbegin();
		int countbraces = 0;
		int poslenta = 0;															// print functions and parametre id and type
		int positionnFunc = 0;
		bool lastifinchai = false;
		for (int i = 0, p = 0; i < posfunc.size(); i++)
		{
			p = posfunc[i];
			
			
#pragma region printID&Parametres
			if (idtable->table[p].idtype == IT::F)
			{
				
				positionnFunc = p;
				file->setf(std::ios::left);
				FILE << idtable->table[p].id;
				FILE << SPACE;
				FILE << PROC;
				FILE << SPACE;

				if (idtable->table[p + 1].idtype == IT::P || idtable->table[p + 1].idtype == IT::PA)
					FILE << COMMA;
				if (i == -1) FILE << ENDL << START;
				p++;
				while (idtable->table[p].idtype == IT::P || idtable->table[p].idtype == IT::PA) 
				{
					FILE << idtable->table[p].id;
					FILE << UNDERLINE;
					FILE << idtable->table[p].prefix;
					FILE << SFP;
					switch (idtable->table[p].iddatatype)
					{
					case IT::CHAR:
					case IT::BOOL:
					{
						FILE << BYTE;
						break;
					}
					case IT::LONG:
					case IT::STR:
					{
						FILE << DWORD;
						break;
					}
					case IT::FLOAT:
					{
						FILE << REAL4;
						break;
					}
					}
					if (idtable->table[p + 1].idtype == IT::P || idtable->table[p + 1].idtype == IT::PA)
						FILE << COMMA << SPACE;
					p++;
				}
				FILE << ENDL << ENDL;
				poslenta = idtable->table[--p].idxfirstLE;
#pragma region findstartfunction
				if (lextable->table[poslenta].lexema[0] == LEX_MAIN) {
					FILE << "push 1251d\n";
					FILE << "call SetConsoleOutputCP\n";
					FILE << "push 1251d\n";
					FILE << "call SetConsoleCP\n";
				}
				while (lextable->table[poslenta].lexema[0] != LEX_LEFTBRACE)
				{
					poslenta++;
				}
				FILE << std::endl;
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
				case LEX_OVERFLOW:
					if (overflow_check)
						overflow_check = false;
					else
						overflow_check = true;
					break;
				case LEX_ID:
				{
					if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::C)
					{
						if (lextable->table[poslenta - 1].lexema[0] != LEX_TYPE && lextable->table[poslenta + 1].lexema[0] == LEX_ASSIGN)
							throw ERROR_THROW_LINE(139, lextable->table[poslenta].sn);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A && lextable->table[poslenta - 1].lexema[0] == LEX_TYPE)
					{
						poslenta = FillArray(lextable, idtable, poslenta);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A || idtable->table[lextable->table[poslenta].idxTI].idtype == IT::PA)
					{
						poslenta = ArrayFillIndex(lextable, idtable, poslenta);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::F || idtable->table[lextable->table[poslenta].idxTI].idtype == IT::E)
					{
						poslenta = PrintCallFunctions(lextable, idtable, poslenta);
					}
					else
					{
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, true, true);
					}
					break;
				}
				case LEX_WRITE:
				case LEX_COMMA:
				{
					poslenta = PrintWriteFunctions(lextable, idtable, poslenta);
					break;
				}
				case LEX_IF:
				{
					poslenta = PrintÑondition(lextable, idtable, poslenta, true, false);
					lastifinchai = true;
					break;
				}
				case LEX_ELSE:
				{
					poslenta = PrintÑondition(lextable, idtable, poslenta + 1, false, false);		//if else
					break;
				}
				case LEX_REPEAT:
				{
					poslenta = PrintCycles(lextable, idtable, poslenta);
					break;
				}
				case LEX_RETURN:
				{
					poslenta = PrintReturn(lextable, idtable, poslenta);
					//if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::PA);
					//FILE << PUSH_EAX;
					break;
				}
				}

				poslenta++;
			} while (countbraces);
			FILE << ENDL;
			FILE << ERROR_ASM;
			if (lextable->table[idtable->table[positionnFunc].idxfirstLE].lexema[0] == LEX_MAIN)
				FILE << EXIT;
			else
				FILE << RET;
			FILE << idtable->table[positionnFunc].id << SPACE << ENDP;
			FILE << std::endl;
		}
#pragma endregion
		FILE << ENDMAIN;
		posfunc.clear();
	}
	int  ProcessingDeclaration(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta, bool logical, bool assign, bool firstcall, bool function)
	{
 		while (lextable->table[poslenta].idxTI == TI_NULLIDX) poslenta++;
		int posID = lextable->table[poslenta].idxTI;
		if (firstcall) poslenta++;
		PrintChain(lextable, idtable, poslenta - 1);
		if (lextable->table[poslenta].lexema[0] == LEX_SEMICOLON)
			return poslenta;
		char ID[32];
		bool registers16 = true, registers32 = true, registersFPU = true;
		REGISTERS  reg32 = eax;
		int stack = 0;
		int static count = 0;

		if (lextable->table[poslenta].lexema[0] == LEX_INCORDEC)
		{
			switch (lextable->table[poslenta].expression[0])
			{
			case ADD:
				FILE << INCREMENT;
				FILE << idtable->table[posID].id;
				FILE << UNDERLINE;
				FILE << idtable->table[posID].prefix << ENDL;
			break;
			case SUB:
				FILE << DECREMENT;
				FILE << idtable->table[posID].id;
				FILE << UNDERLINE;
				FILE << idtable->table[posID].prefix << ENDL;
			break;
			}
			return poslenta + 1;
		}

		while (true) {
			std::string temp;
			switch (lextable->table[poslenta].lexema[0])
			{
			case LEX_ID:
			{
				switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
				{
				case IT::A:
				case IT::PA:
				{
					if (true) {
						int posarray = poslenta;
						switch (reg32) {
						case ebx:
						{
							FILE << PUSH_EAX;
							break;
						}
						case ecx:
						{
							FILE << PUSH_EAX << PUSH_EBX;
						}
						}
						if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A) {
							FILE << MOV_ECX_ADDR << idtable->table[lextable->table[poslenta].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
						}
						else 
						{
							FILE << MOV_ECX << idtable->table[lextable->table[poslenta].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
						}
						FILE << PUSH_ECX;
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, true, true);					// ðåêóðñèÿ 
						FILE << POP_ECX;
						if (idtable->table[lextable->table[posarray].idxTI].iddatatype == IT::LONG) 
						{
							FILE << IMUL_EAX_TYPE;
							FILE << idtable->table[lextable->table[posarray].idxTI].id;
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
						}
						if (idtable->table[lextable->table[posarray].idxTI].iddatatype == IT::CHAR) 
						{
							FILE << XOR_EBX;
							FILE << MOV_BL_AL;
							FILE << MOV_EAX_EBX;
							FILE << IMUL_EAX_TYPE;
							FILE << idtable->table[lextable->table[posarray].idxTI].id;
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
						}
						
						FILE << ADD_ECX_EAX;

						if(function){
						FILE << MOV_EDX << PREFERRED_ECX;

						switch (reg32) {
						case ebx:
						{
							FILE << POP_EAX;
							FILE << MOV_EBX << EDX << ENDL;
							reg32 = ecx;
							break;
						}
						case ecx:
						{
							FILE << POP_EAX << POP_EBX;
							FILE << PUSH_EDX;
							stack++;
							break;
						}
						default:
						{
							FILE << MOV_EAX_EDX;
							reg32 = ebx;
							break;
						}
						}
						}
					}
					if (logical || !function)
						return poslenta;
					break;
				}
				case IT::C:
				{
						switch (reg32)
						{
						case eax:
						{
							FILE << MOV_EAX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							reg32 = ebx;
							break;
						}
						case ebx:
						{
							FILE << MOV_EBX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							reg32 = ecx;
							break;
						}
						case ecx:
						{
							FILE << MOV_ECX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							FILE << PUSH_ECX;
							stack++;
							break;
						}
						}

						break;
				}
				case IT::P:
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
							FILE << MOV_EAX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							reg32 = ebx;
							break;
						}
						case ebx:
						{
							FILE << MOV_EBX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							reg32 = ecx;
							break;
						}
						case ecx:
						{
							FILE << MOV_ECX;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							FILE << PUSH_ECX;
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
							FILE << XOR_EAX;
							FILE << MOV_AL;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							reg32 = ebx;
							break;
						}
						case ebx:
						{
							FILE << MOV_BL;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							reg32 = ecx;
							break;
						}
						case ecx:
						{
							FILE << MOV_CL;
							temp += idtable->table[lextable->table[poslenta].idxTI].id;
							temp += UNDERLINE;
							temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
							FILE << temp;
							FILE << ENDL;
							FILE << PUSH_ECX;
							stack++;
							break;
						}
						}
						break;
					}
					case IT::STR:
					{
						
					}
						break;
					case IT::FLOAT:
					{
						break;
					}
					}
					break;
				}
				case IT::E:
				case IT::F:
				{
					switch (reg32) {
					case ebx:
					{
						FILE << PUSH_EAX;
						break;
					}
					case ecx:
					{
						FILE << PUSH_EAX << PUSH_EBX;
						break;
					}
					}
					poslenta = PrintCallFunctions(lextable, idtable, poslenta);
					switch (reg32) {
					case ebx:
					{
						FILE << MOV_EBX_EAX;										// save result function
						FILE << POP_EAX;
						reg32 = ecx;
						break;
					}
					case ecx:
					{
						FILE << MOV_ECX_EAX;										// save result function				
						FILE  << POP_EBX ;
						FILE << PUSH_ECX;
						stack++;
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
						FILE << MOV_EAX;
						temp = LITERALS;
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						FILE << temp;
						FILE << ENDL;
						reg32 = ebx;
						break;
					}
					case ebx:
					{
						FILE << MOV_EBX;
						temp = LITERALS;
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						FILE << temp;
						FILE << ENDL;
						reg32 = ecx;
						break;
					}
					case ecx:
					{
						FILE << MOV_ECX;
						temp = LITERALS;
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						FILE << temp;
						FILE << ENDL;
						FILE << PUSH_ECX;
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
						FILE << MOV_AL;
						temp = LITERALS;
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						FILE << temp;
						FILE << ENDL;
						reg32 = ebx;
						break;
					}
					case ebx:
					{
						FILE << MOV_BL;
						temp = LITERALS;
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						FILE << temp;
						FILE << ENDL;
						reg32 = ecx;
						break;
					}
					case ecx:
					{
						FILE << MOV_CL;
						temp = LITERALS;
						temp += UNDERLINE;
						temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
						FILE << temp;
						FILE << ENDL;
						FILE << PUSH_ECX;
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
				case IT::STR:
				{
					if (lextable->table[poslenta - 2].idxTI != TI_NULLIDX) {
						if (idtable->table[lextable->table[poslenta - 2].idxTI].idtype == IT::V)
						{
							char buffer[32];
							FILE << PUSH_OFFSET;
							FILE << LITERALS;
							FILE << UNDERLINE;
							FILE << itoa(lextable->table[poslenta].idxTI, buffer, 10) << ENDL;
							FILE << PUSH_LENGHTOFF;
							FILE << LITERALS;
							FILE << UNDERLINE;
							FILE << itoa(lextable->table[poslenta].idxTI, buffer, 10) << ENDL;
							FILE << CLD;
							FILE << POP_ECX;
							FILE << POP_ESI;
							FILE << LEA_EDI;
							FILE << idtable->table[lextable->table[poslenta-2].idxTI].id;
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[poslenta-2].idxTI].prefix << ENDL;
							FILE << REP_MOVSB;
						}
						break;
					}
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
						FILE << ADD_EAX_EBX;
						if(overflow_check) FILE << OVERFLOW_ASM;
						break;
					}
					case ecx:
					{
						if (stack > 1)
						{
							FILE << POP_ECX << POP_EDX;
							FILE << ADD_ECX_EDX;
							if(overflow_check) FILE << OVERFLOW_ASM;
							FILE << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							FILE << POP_ECX;
							FILE << ADD_EBX_ECX;
							if(overflow_check) FILE << OVERFLOW_ASM;
							stack--;
							reg32 = ebx;
						}
						else {
							FILE << ADD_EAX_EBX;
							if(overflow_check) FILE << OVERFLOW_ASM;
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
						FILE << SUB_EAX_EBX;
						if(overflow_check) FILE << OVERFLOW_ASM;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							FILE << POP_ECX << POP_EDX;
							FILE << SUB_ECX_EDX;
							if(overflow_check) FILE << OVERFLOW_ASM;
							FILE << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							FILE << POP_ECX;
							FILE << SUB_EBX_ECX;
							if(overflow_check) FILE << OVERFLOW_ASM;
							stack--;
							reg32 = ebx;
						}
						else {
							FILE << SUB_EAX_EBX;
							if(overflow_check) FILE << OVERFLOW_ASM;
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
						FILE << CMP_EBX_0;
						FILE << XOR_EDX;
						FILE << DIV_EAX_EBX;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							FILE << POP_ECX << POP_EDX;
							FILE << CMP_EBX_0;
							FILE << PUSH_EAX;
							FILE << MOV_EAX_ECX;
							FILE << DIV_EDX;
							FILE << MOV_ECX_EAX;
							FILE << POP_EAX;
							FILE << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							FILE << POP_ECX;
							FILE << CMP_EBX_0;
							FILE << PUSH_EAX;
							FILE << MOV_EAX_EBX;
							FILE << XOR_EDX;
							FILE << DIV_EAX_ECX;
							FILE << MOV_EBX_EAX;
							FILE << POP_EAX;
							stack--;
							reg32 = ebx;
						}
						else {
							FILE << CMP_EBX_0;
							FILE << XOR_EDX;
							FILE << DIV_EAX_EBX;
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
						FILE << MUL_EAX_EBX;
						if (overflow_check) FILE << OVERFLOW_ASM;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							if (overflow_check) 
							{
								FILE << POP_ECX << POP_EDX;
								FILE << PUSH_EAX;
								FILE << PUSH_EBX;
								FILE << MOV_EAX_ECX;
								FILE << MOV_EBX_EDX;

								FILE << MUL_EAX_EBX;
								FILE << OVERFLOW_ASM;
								FILE << MOV_ECX_EAX;
								FILE << POP_EBX;
								FILE << POP_EAX;
								FILE << PUSH_ECX;
							}
							else {
								FILE << POP_ECX << POP_EDX;
								FILE << MUL_ECX_EDX;
								FILE << PUSH_ECX;
							}
							stack--;
						}
						else if (stack == 1)
						{
							if (overflow_check)
							{
								FILE << POP_ECX;
								FILE << PUSH_EAX;
								FILE << MOV_EAX_EBX;
								FILE << MUL_EAX_ECX;
								FILE << MOV_EBX_EAX;
								FILE << POP_EAX;

							}
							else 
							{
								FILE << POP_ECX;
								FILE << MUL_EBX_ECX;
								if (overflow_check) FILE << OVERFLOW_ASM;
							}
							stack--;
							reg32 = ebx;
						}
						else {
							FILE << MUL_EAX_EBX;
							if (overflow_check) FILE << OVERFLOW_ASM;
							reg32 = ebx;
						}
						break;
					}
					}
					break;
				
				}
				break;

			case LEX_SEMICOLON:
			{	switch(idtable->table[posID].idtype)
				{
				case IT::V:
				case IT::P:
					switch (idtable->table[posID].iddatatype)
					{
					case IT::LONG:
					{
						FILE << MOV;
						FILE << idtable->table[posID].id;
						FILE << UNDERLINE;
						FILE << idtable->table[posID].prefix;
						FILE << EAX << ENDL;
						break;
					}
					case IT::BOOL:
					case IT::CHAR:
					{
						if (overflow_check) FILE << OVERFLOW_ASM_BYTE;
						FILE << MOV;
						FILE << idtable->table[posID].id;
						FILE << UNDERLINE;
						FILE << idtable->table[posID].prefix;
						FILE << AL << ENDL;
						break;
					}
					case IT::FLOAT:
					{
						break;
					}
					}
				}
			FILE << ENDL;
			}
			case LEX_SQUARE_RIGHT:
			case LEX_COMMA:
			case LOGICALS:
			{
				return poslenta;
				break;
			}
			case LEX_RIGHTESIS:
				return poslenta-1;
				break;
			}
			poslenta++;
			
		}
		return poslenta-1;
	}
	int  FillArray(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		PrintChain(lextable, idtable, poslenta - 1);
		bool start = false;
		bool first = false;
		int count = 0;
		char ID[32];
		FILE << XOR_EAX;
		FILE << MOV_EBX_TYPE << idtable->table[lextable->table[poslenta].idxTI].id;
		FILE << UNDERLINE;
		FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
		FILE << MOV_ECX_ADDR << idtable->table[lextable->table[poslenta].idxTI].id;
		FILE << UNDERLINE;
		FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL << ENDL;

		std::string temp;
		while (lextable->table[poslenta++].lexema[0] != LEX_SEMICOLON)
		{
			if (lextable->table[poslenta].lexema[0] == LEX_LEFTBRACE)
				start = true;
			if (start)
			{	if(lextable->table[poslenta].idxTI!=TI_NULLIDX)
				if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::L) {
					FILE << SEMICOLON << idtable->table[lextable->table[poslenta].idxTI].id << LEFTSQUARE << count << RIGHTSQUARE << ASSIGN << idtable->table[lextable->table[poslenta].idxTI].value.vint << ENDL;
					if(first) FILE << ADD_EAX_EBX;
					temp = LITERALS ;
					temp += UNDERLINE;
					temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
					FILE << MOV_EDX << temp << ENDL;
					FILE << MOV_ECX_EAX_EDX;
					FILE << ENDL;
					first = true;
					count++;
				}

			}
		}
		return poslenta -1;

	}
	int  ArrayFillIndex(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta) {
		//PrintChain(lextable, idtable, poslenta - 1);
		int posID = poslenta;

		while (lextable->table[poslenta].lexema[0] != LEX_SEMICOLON)
		{
			if(lextable->table[poslenta++].lexema[0] == LEX_ASSIGN)
				poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false, true);
		}
		FILE << PUSH_EAX;
		if (idtable->table[lextable->table[posID].idxTI].idtype == IT::A) {
			FILE << MOV_ECX_ADDR << idtable->table[lextable->table[posID].idxTI].id;	    // àäðåññ ìàññèâà asm
			FILE << UNDERLINE;
			FILE << idtable->table[lextable->table[posID].idxTI].prefix << ENDL;
		}
		else
		{
			FILE << MOV_ECX << idtable->table[lextable->table[posID].idxTI].id;	    // àäðåññ ìàññèâà asm
			FILE << UNDERLINE;
			FILE << idtable->table[lextable->table[posID].idxTI].prefix << ENDL;
		}
		if(idtable->table[lextable->table[posID].idxTI].iddatatype == IT::STR)
			FILE << MOV_EBX << BYTE << ENDL;
		else
			FILE << MOV_EBX_TYPE << idtable->table[lextable->table[posID].idxTI].id << UNDERLINE << idtable->table[lextable->table[posID].idxTI].prefix << ENDL;
		FILE << PUSH_EBX;
		ProcessingDeclaration(lextable, idtable, posID+1, false, false, false, true);
		FILE << POP_EBX;
		FILE << MUL_EAX_EBX;
		FILE << ADD_ECX_EAX;
		FILE << POP_EAX;
		switch (idtable->table[lextable->table[posID].idxTI].iddatatype)
		{
		case IT::STR:
			FILE << MOV << PREFERRED_ECX_C << AL << ENDL;
			break;
		case IT::LONG:
		FILE << MOV << PREFERRED_ECX_C << EAX << ENDL;
			break;
		case IT::CHAR:
		case IT::BOOL:
			if (overflow_check) FILE << OVERFLOW_ASM_BYTE;
			FILE << MOV << PREFERRED_ECX_C << EAX << ENDL;
			break;
		}

		return poslenta-1;
	}
	int  PrintWriteFunctions(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		//PrintChain(lextable, idtable, poslenta - 1);
		int posID = lextable->table[poslenta].idxTI;
		bool atLeatsOneParametres = false;
		int posfordatatype = 0;
		while (lextable->table[poslenta++].lexema[0] != LEX_SEMICOLON)
		{

			if (lextable->table[poslenta].idxTI != TI_NULLIDX) {
				PrintChain(lextable, idtable, poslenta - 2);
				atLeatsOneParametres = true;
				switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
				{
				case IT::L:
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					case IT::CHAR:
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, true, false, true);
						FILE << "invoke int_to_char, ADDR output, eax\n";// << LITERALS  << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;

						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, ADDR output\n";
						else
							FILE << "invoke writeline, ADDR output\n";
						break;
					case IT::FLOAT:
						break;
					case IT::STR:

						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, ADDR ";
						else
							FILE << "invoke writeline, ADDR ";
						FILE << LITERALS  << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;
						break;
					}
					break;
				case IT::V:
				case IT::P:
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					case IT::CHAR:
						FILE << "invoke int_to_char, ADDR output," 
							<< idtable->table[lextable->table[poslenta].idxTI].id 
							<< UNDERLINE << idtable->table[lextable->table[poslenta].idxTI].prefix 
							<< ENDL;						
						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, ADDR output\n";
						else
							FILE << "invoke writeline, ADDR output\n";
						break;
					case IT::FLOAT:
						break;
					case IT::STR:
						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, ADDR ";
						else
							FILE << "invoke writeline, ADDR ";
						FILE << idtable->table[lextable->table[poslenta].idxTI].id
							<< UNDERLINE << idtable->table[lextable->table[poslenta].idxTI].prefix
							<< ENDL;
						break;
					}

					break;
				case IT::E:
				case IT::F:
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					case IT::CHAR:
						poslenta = PrintCallFunctions(lextable, idtable, poslenta);
						FILE << "invoke int_to_char, ADDR output" << EAX << ENDL;
						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, ADDR output\n";
						else
							FILE << "invoke writeline, ADDR output\n";
						break;
					case IT::FLOAT:
						break;
					case IT::STR:
						poslenta = PrintCallFunctions(lextable, idtable, poslenta);
						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, eax ";
						else
							FILE << "invoke writeline, eax ";
						break;
					}

					break;
				case IT::A:
					posfordatatype = poslenta;
					poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false, true);
					FILE << "invoke int_to_char, ADDR output";
					switch (idtable->table[lextable->table[posfordatatype].idxTI].iddatatype)
					{
					case IT::LONG:
						FILE << EAX << ENDL;
						break;
					case IT::CHAR:
						FILE << AL << ENDL;
						break;
					}
					if (idtable->table[posID].value.vbool)
						FILE << "invoke write, ADDR output\n";
					else
						FILE << "invoke writeline, ADDR output\n";
					FILE << ENDL;
					break;
				case IT::PA:
					posfordatatype = poslenta;
					switch (idtable->table[lextable->table[posfordatatype].idxTI].iddatatype)
					{
					case IT::LONG:
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false, true);
						FILE << "invoke int_to_char, ADDR output,[ecx]\n";
						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, ADDR output\n";
						else
							FILE << "invoke writeline, ADDR output\n";
						FILE << ENDL;
						break;
					case IT::STR:
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false, true);
						if (idtable->table[posID].value.vbool)
							FILE << "invoke write, ADDR [ecx]\n";
						else
							FILE << "invoke writeline, [ecx]\n";
						FILE << ENDL;
						break;
					}
				}
			}

		}
		if(!atLeatsOneParametres) FILE << "invoke write, ADDR endl\n"; // not at leats one parametres
		return poslenta - 1;
	}
	int  PrintÑondition(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta, bool lexif, bool cycle)
	{
		PrintChain(lextable, idtable, poslenta - 1);
		int localidlabel = CountCondition++;
		int countbraces = 0;
		int countparametres = 0;
		bool eax = true;
		int poslogical = 0;
		bool foundbool = true;

		if (lextable->table[poslenta+2].idxTI != TI_NULLIDX)
		if (idtable->table[lextable->table[poslenta+2].idxTI].iddatatype == IT::BOOL)
		{
			FILE << XOR_EAX;
			FILE << MOV_AL;
			FILE << idtable->table[lextable->table[poslenta+2].idxTI].id;
			FILE << UNDERLINE;
			FILE << idtable->table[lextable->table[poslenta+2].idxTI].prefix << ENDL;
			FILE << MOV_EBX << "1h" << ENDL;
			FILE << CMP_EAX_EBX;
			FILE << JL_TO_F << localidlabel <<  ENDL;
			foundbool = false;
			poslenta += 4;
		}
		else if (lexif || cycle)
		{
			while (lextable->table[poslenta].lexema[0] != LEX_LEFTBRACE && lextable->table[poslenta].lexema[0] != LEX_SEMICOLON) {
				if (lextable->table[poslenta].lexema[0] == LOGICALS)
					poslogical = poslenta;
				if (lextable->table[poslenta].idxTI != TI_NULLIDX) {
					switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
					{
					case IT::L:
						switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
						{
						case IT::LONG:
							if (eax)	FILE << MOV_EAX;
							else		FILE << MOV_EBX;
							FILE << LITERALS << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;
							break;
						case IT::STR:
							throw ERROR_THROW_LINE(140, lextable->table[poslenta].sn);
							break;
						}
						break;
					case IT::V:
						switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
						{
						case IT::LONG:
							if (eax) FILE << MOV_EAX;
							else	 FILE << MOV_EBX;
							FILE << idtable->table[lextable->table[poslenta].idxTI].id;
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
							break;
						case IT::CHAR:
						case IT::BOOL:
							if (eax)FILE << MOV_AL;
							else	FILE << MOV_BL;
							FILE << idtable->table[lextable->table[poslenta].idxTI].id;
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
							break;
						case IT::STR:
							throw ERROR_THROW_LINE(140, lextable->table[poslenta].sn);
							break;
						}
						break;
					case IT::A:
					case IT::PA:
						if (!eax) FILE << PUSH_EAX << ENDL;
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, true, true, false, true);
						if (!eax) {
							FILE << POP_EBX;
						}
						break;

					}
					eax = false;
				}
				poslenta++;
			}

			if (foundbool) {
				
			if(lexif)
				FILE << CMP_EAX_EBX;
				switch (lextable->table[poslogical].expression[0])
				{
				case '<':
				{	if (lextable->table[poslogical].expression[1])
					FILE << JBE_TO_F << localidlabel <<  ENDL;
				else
					FILE << JLE_TO_F << localidlabel <<  ENDL;
				break;
				}
				case '>':
				{
					if (lextable->table[poslogical].expression[1])
						FILE << JL_TO_F << localidlabel << ENDL;
					else
						FILE << JGE_TO_F << localidlabel <<  ENDL;
					break;
				}
				case '=':
				{
					FILE << JNE_TO_F << localidlabel <<  ENDL;
					break;
				}
				case '!':
				{
					FILE << JE_TO_F	 << localidlabel <<  ENDL;
					break;
				}
				}
			}
		}
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
					if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::C) {
						if (lextable->table[poslenta - 1].lexema[0] != LEX_TYPE && lextable->table[poslenta + 1].lexema[0] == LEX_ASSIGN)
							throw ERROR_THROW_LINE(139, lextable->table[poslenta].sn);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A && lextable->table[poslenta - 1].lexema[0] == LEX_TYPE)
					{
						poslenta = FillArray(lextable, idtable, poslenta);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A|| idtable->table[lextable->table[poslenta].idxTI].idtype == IT::PA) {
						poslenta = ArrayFillIndex(lextable, idtable, poslenta);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::F || idtable->table[lextable->table[poslenta].idxTI].idtype == IT::E)
					{
						poslenta = PrintCallFunctions(lextable, idtable, poslenta);
					}
					else {
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, true, true);
					}
					break;
				}
				case LEX_WRITE:
				case LEX_COMMA:
				{
					poslenta = PrintWriteFunctions(lextable, idtable, poslenta);
					break;
				}
				case LEX_IF:
				{
					poslenta = PrintÑondition(lextable, idtable, poslenta, true,false);
					break;
				}
				case LEX_ELSE:
				{

					poslenta = PrintÑondition(lextable, idtable, poslenta+1, false,false);		//if else
					break;
				}
				case LEX_REPEAT:
				{
					poslenta = PrintCycles(lextable, idtable, poslenta);
					break;
				}
				}

				poslenta++;
			} while (countbraces);
			if (lextable->table[poslenta].lexema[0] == LEX_ELSE)
				FILE <<JUMP_TO_T << localidlabel << ENDL;
			if(lexif)
				FILE << LABEL_F << localidlabel << COLON << ENDL;
			else
				FILE << LABEL_T << localidlabel - 1  << COLON << ENDL;
		return poslenta - 1;
	}
	int  PrintCycles(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		PrintChain(lextable, idtable, poslenta - 1);
		int localidlabel = CountCondition++;
		int countbraces = 0;
		bool eax = true;
		int poslogical;
		FILE << LABEL_F << localidlabel << COLON << ENDL;
			do
			{
				if (lextable->table[++poslenta].lexema[0] == LEX_LEFTBRACE)
					countbraces++;
				if (lextable->table[poslenta].lexema[0] == LEX_RIGHTBRACE)
					countbraces--;

				switch (lextable->table[poslenta].lexema[0])
				{

				case LEX_ID:
				{
					if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::C) {
						if (lextable->table[poslenta - 1].lexema[0] != LEX_TYPE && lextable->table[poslenta + 1].lexema[0] == LEX_ASSIGN)
							throw ERROR_THROW_LINE(139, lextable->table[poslenta].sn);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A && lextable->table[poslenta - 1].lexema[0] == LEX_TYPE)
					{
						poslenta = FillArray(lextable, idtable, poslenta);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A || idtable->table[lextable->table[poslenta].idxTI].idtype == IT::PA) {
						poslenta = ArrayFillIndex(lextable, idtable, poslenta);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::F || idtable->table[lextable->table[poslenta].idxTI].idtype == IT::E)
					{
						poslenta = PrintCallFunctions(lextable, idtable, poslenta);
					}
					else {
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, true, true);
					}
					break;
				}
				case LEX_WRITE:
				case LEX_COMMA:
				{
					poslenta = PrintWriteFunctions(lextable, idtable, poslenta);
					break;
				}
				case LEX_IF:
				{
					poslenta = PrintÑondition(lextable, idtable, poslenta, true,false);
					break;
				}
				case LEX_ELSE:
				{
					poslenta = PrintÑondition(lextable, idtable, poslenta, false, false);		//if else
					break;
				}
				case LEX_REPEAT:
				{
					poslenta = PrintCycles(lextable, idtable, poslenta);
					break;
				}
				}
			} while (countbraces);

			if (lextable->table[poslenta + 1].lexema[0] == LEX_UNTIL) 
			{
				while (lextable->table[poslenta].lexema[0] != LEX_LEFTBRACE && lextable->table[poslenta].lexema[0] != LEX_SEMICOLON) {
					if (lextable->table[poslenta].lexema[0] == LOGICALS)
						poslogical = poslenta;
					if (lextable->table[poslenta].idxTI != TI_NULLIDX) {
						switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
						{
						case IT::L:
							switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
							{
							case IT::LONG:
								if (eax)	FILE << MOV_EAX;
								else		FILE << MOV_EBX;
								FILE << LITERALS << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;
								break;
							case IT::STR:
								throw ERROR_THROW_LINE(140, lextable->table[poslenta].sn);
								break;
							}
							break;
						case IT::V:
						case IT::P:
							switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
							{
							case IT::LONG:
								if (eax) FILE << MOV_EAX;
								else	 FILE << MOV_EBX;
								FILE << idtable->table[lextable->table[poslenta].idxTI].id;
								FILE << UNDERLINE;
								FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
								break;
							case IT::CHAR:
							case IT::BOOL:
								if (eax) {
									FILE << XOR_EAX;
									FILE << MOV_AL;
								}
								else
								{
									FILE << XOR_EBX;
									FILE << MOV_BL;
								}
								FILE << idtable->table[lextable->table[poslenta].idxTI].id;
								FILE << UNDERLINE;
								FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
								break;
							case IT::STR:
								throw ERROR_THROW_LINE(140, lextable->table[poslenta].sn);
								break;
							}
							break;
						case IT::A:
							poslenta = ProcessingDeclaration(lextable, idtable, poslenta, true, true, false, true);
							break;

						}
						eax = false;
					}
					poslenta++;
				}
					FILE << CMP_EAX_EBX;

					switch (lextable->table[poslogical].expression[0])
					{
					case '<':
					{	if (lextable->table[poslogical].expression[1])
						FILE << JBE_TO_F << localidlabel <<  ENDL;
					else
						FILE << JLE_TO_F << localidlabel <<  ENDL;
					break;
					}
					case '>':
					{
						if (lextable->table[poslogical].expression[1])
							FILE << JL_TO_F << localidlabel << ENDL;
						else
							FILE << JGE_TO_F << localidlabel <<  ENDL;
						break;
					}
					case '=':
					{
						FILE << JE_TO_F	 << localidlabel <<  ENDL;
						break;
					}
					case '!':
					{
						FILE << JNE_TO_F << localidlabel <<  ENDL;
						break;
					}
					}
			}
			return poslenta-1;
	}
	int  PrintCallFunctions(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		int pos = poslenta;
		int ID = poslenta;
		bool anyparametres = false;
		int countParametres = 1;

		while (lextable->table[poslenta++].lexema[0] != LEX_RIGHTESIS)
		{
			if (lextable->table[poslenta].idxTI != TI_NULLIDX && !anyparametres)
				if (
					idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A ||
					idtable->table[lextable->table[poslenta].idxTI].idtype == IT::V ||
					idtable->table[lextable->table[poslenta].idxTI].idtype == IT::L ||
					idtable->table[lextable->table[poslenta].idxTI].idtype == IT::C
					)
					anyparametres = true;
			if(lextable->table[poslenta].lexema[0] == LEX_COMMA)
				countParametres++;
		
		}
		if (idtable->table[lextable->table[ID].idxTI].value.vint == countParametres)
		{
			pos = poslenta-1;
			while (lextable->table[pos--].lexema[0] != LEX_LEFTHESIS)
			{
				if (lextable->table[pos].lexema[0] == LEX_COMMA)
				{
					switch (idtable->table[(lextable->table[pos + 1].idxTI)].idtype)
					{
					case IT::A:

						if (lextable->table[pos + 2].lexema[0] == LEX_SQUARE_LEFT)
						{
							ProcessingDeclaration(lextable, idtable, pos + 1, false, false, false, true);
							if (idtable->table[lextable->table[pos + 1].idxTI].iddatatype == IT::CHAR) {
								FILE << XOR_EBX;
								FILE << MOV_BL_AL;
								FILE << MOV_EAX_EBX;
								FILE << IMUL_EAX_TYPE;
							}
							FILE << PUSH_EAX;
						}
						else
						{
							FILE << MOV_ECX_ADDR << idtable->table[lextable->table[pos + 1].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[pos + 1].idxTI].prefix << ENDL;
							FILE << PUSH_ECX;
						}
						break;
					case IT::L:
					{
						switch (idtable->table[(lextable->table[pos + 1].idxTI)].iddatatype)
						{
						case IT::STR:
							FILE << MOV_ECX_ADDR << LITERALS << UNDERLINE;
							FILE << lextable->table[pos + 1].idxTI << ENDL;
							FILE << PUSH_ECX;
							break;
						case IT::LONG:
						case IT::CHAR:
							FILE << MOV_ECX << LITERALS << UNDERLINE;
							FILE << lextable->table[pos + 1].idxTI << ENDL;
							FILE << PUSH_ECX;
							break;
						}
						break;
					}
					case IT::V:
					{
						switch (idtable->table[(lextable->table[pos + 1].idxTI)].iddatatype)
						{
						case IT::STR:
							FILE << MOV_ECX_ADDR << idtable->table[lextable->table[pos + 1].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[pos + 1].idxTI].prefix << ENDL;
							FILE << PUSH_ECX;
							break;
						case IT::LONG:
						case IT::CHAR:
							FILE << MOV_ECX << idtable->table[lextable->table[pos + 1].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[pos + 1].idxTI].prefix << ENDL;
							FILE << PUSH_ECX;
							break;
						}
						break;
					}
					break;
					default:
					{
						ProcessingDeclaration(lextable, idtable, pos + 1, false, false, false, false);
						FILE << PUSH_EAX;
					}
					}
				}
				else if (lextable->table[pos].lexema[0] == LEX_LEFTHESIS) 
				{
					switch (idtable->table[(lextable->table[pos + 1].idxTI)].idtype)
					{
					case IT::A:

						if (lextable->table[pos + 2].lexema[0] == LEX_SQUARE_LEFT)
						{
							ProcessingDeclaration(lextable, idtable, pos + 1, false, false, false, true);
							if (idtable->table[lextable->table[pos + 1].idxTI].iddatatype == IT::CHAR) {
								FILE << XOR_EBX;
								FILE << MOV_BL_AL;
								FILE << MOV_EAX_EBX;
								FILE << IMUL_EAX_TYPE;
							}
							FILE << PUSH_EAX;
						}
						else
						{
							FILE << MOV_ECX_ADDR << idtable->table[lextable->table[pos + 1].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[pos + 1].idxTI].prefix << ENDL;
							FILE << PUSH_ECX;
						}
						break;
					case IT::L:
					{
						switch (idtable->table[(lextable->table[pos + 1].idxTI)].iddatatype)
						{
						case IT::STR:
							FILE << MOV_ECX_ADDR << LITERALS << UNDERLINE;
							FILE << lextable->table[pos + 1].idxTI << ENDL;
							FILE << PUSH_ECX;
							break;
						case IT::LONG:
						case IT::CHAR:
							FILE << MOV_ECX << LITERALS << UNDERLINE;
							FILE << lextable->table[pos + 1].idxTI << ENDL;
							FILE << PUSH_ECX;
							break;
						}
						break;
					}
					case IT::V:
					{
						switch (idtable->table[(lextable->table[pos + 1].idxTI)].iddatatype)
						{
						case IT::STR:
							FILE << MOV_ECX_ADDR << idtable->table[lextable->table[pos + 1].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[pos + 1].idxTI].prefix << ENDL;
							FILE << PUSH_ECX;
							break;
						case IT::LONG:
						case IT::CHAR:
							FILE << MOV_ECX << idtable->table[lextable->table[pos + 1].idxTI].id;	    // àäðåññ ìàññèâà asm
							FILE << UNDERLINE;
							FILE << idtable->table[lextable->table[pos + 1].idxTI].prefix << ENDL;
							FILE << PUSH_ECX;
							break;
						}
						break;
					}
						break;
					default:
					{
						ProcessingDeclaration(lextable, idtable, pos + 1, false, false, false, false);
						FILE << PUSH_EAX;
					}
					}
				}
			}
			FILE << CALL << idtable->table[lextable->table[ID].idxTI].extfunct <<
							 idtable->table[lextable->table[ID].idxTI].id << ENDL;
		}
		else
			throw ERROR_THROW_LINE(141, lextable->table[ID].sn);
		return poslenta-1;
	}
	int  PrintReturn(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		int posID = lextable->table[poslenta+1].idxTI;
		int posfordatatype = 0;
		while (lextable->table[poslenta++].lexema[0] != LEX_SEMICOLON)
		{

			if (lextable->table[poslenta].idxTI != TI_NULLIDX) {
				PrintChain(lextable, idtable, poslenta - 2);
				switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
				{
				case IT::V:
				case IT::P:
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					case IT::CHAR:
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false, true);
						break;
					case IT::FLOAT:
						break;
					case IT::STR:
						FILE << MOV_EAX << OFFSET;
						FILE << idtable->table[lextable->table[poslenta].idxTI].id;
						FILE << UNDERLINE;
						FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
						break;
					}
					break;
				case IT::L:
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					case IT::CHAR:
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, true, false, true);
						break;
					case IT::FLOAT:
						break;
					case IT::STR:
						FILE << MOV_EAX << OFFSET;
						FILE << LITERALS << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;
						break;
					}
					break;
				case IT::A:
					posfordatatype = poslenta;
					poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false, true);
					//switch (idtable->table[lextable->table[posfordatatype].idxTI].iddatatype)
					//{
					//case IT::LONG:
					//	FILE << EAX << ENDL;
					//	break;
					//case IT::CHAR:
					//	FILE << AL << ENDL;
					//	break;
					//}
					break;
				case IT::PA:
					if(lextable->table[poslenta - 1].lexema[0] == RIGHTSQUARE)
					poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false, true);
					FILE << MOV_EAX;
					FILE << idtable->table[lextable->table[poslenta].idxTI].id;
					FILE << UNDERLINE;
					FILE << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
					break;
				}
			}

		}

		return poslenta-1;
	}
	void PrintChain(LT::LexTable* lextable, IT::IdTable* idtable, int pos)
	{
		int poslenta = pos;
		FILE << SEMICOLON;
		FILE << LEFTHESIS << lextable->table[poslenta].sn << RIGHTESIS;
		while (lextable->table[poslenta].lexema[0] != SEMICOLON)
		{
			if (lextable->table[poslenta].lexema[0] == '©')
			{
				FILE << lextable->table[poslenta++].lexema[0];
				continue;
			}
			else if (lextable->table[poslenta].idxTI == TI_NULLIDX)
			{
				if (lextable->table[poslenta].expression[0])
				{
					FILE << lextable->table[poslenta].expression;
				}
				else
				{
					FILE << lextable->table[poslenta].lexema[0];
				}
				FILE << SPACE;
			}
			else
			{
				if (idtable->table[lextable->table[poslenta].idxTI].id[0])
				{
					FILE << idtable->table[lextable->table[poslenta].idxTI].id;
					FILE << SPACE;
				}
				else
				{
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					{
						FILE << idtable->table[lextable->table[poslenta].idxTI].value.vint;
						break;
					}
					case IT::CHAR:
					{
						FILE << idtable->table[lextable->table[poslenta].idxTI].value.vchar;
						break;
					}
					case IT::BOOL:
					{
						FILE << idtable->table[lextable->table[poslenta].idxTI].value.vbool;
						break;
					}
					case IT::FLOAT:

					{
						FILE << idtable->table[lextable->table[poslenta].idxTI].value.vfloat;
						break;
					}
					case IT::STR:
					{
						FILE << idtable->table[lextable->table[poslenta].idxTI].value.vstr->str;
						break;
					}

					}
					FILE << SPACE;
				
				}
			}
			poslenta++;
		}
		FILE << ENDL;
	}
}


