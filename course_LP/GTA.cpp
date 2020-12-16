#include "stdafx.h"

namespace GTA
{
	bool warning_lvl = true;
	static int CountCondition = 0;
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
		PrintConst(lextable, idtable);
		*file << DATA;
		*file << SYSTEMERRORS;
		PrintLiterals(lextable, idtable);
		PrintVariables(lextable, idtable);
		*file << COMMENTCODE;
		*file << CODE;
		*file << FUNCTION_ERROR;
		*file << RANDOME_FUNC;
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
					*file << temp;
					file->width(10);
					*file << "DWORD";
					*file << idtable->table[lextable->table[i].idxTI].value.vint;
					*file << std::endl;
					break;
				}
				case IT::FLOAT:
				{
					std::string temp = LITERALS ;
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
					std::string temp = LITERALS ;
					temp += UNDERLINE;
					temp += itoa(lextable->table[i].idxTI, buffer, 10);
					file->setf(std::ios::left);
					file->width(24);
					*file << temp;
					file->width(10);
					*file << "BYTE";
					*file << "\"";
					*file << idtable->table[lextable->table[i].idxTI].value.vstr->str;
					*file << "\"," << STR_END ;
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
	void PrintConst(LT::LexTable* lextable, IT::IdTable* idtable) 
	{
		*file << USERCONST;
		for (int i = 0; i < idtable->current_size; i++)
		{
			if (idtable->table[i].idtype == IT::C) {
				std::string temp;
				temp += idtable->table[i].id;
				temp += UNDERLINE;
				temp += idtable->table[i].prefix;
				file->setf(std::ios::left);
				file->width(24);
				*file << temp;
				file->width(10);
				if (lextable->table[idtable->table[i].idxfirstLE + 1].lexema[0] == LEX_ASSIGN)
				{
					switch (idtable->table[i].iddatatype)
					{
					case IT::LONG:
						*file << EQU << idtable->table[i + 1].value.vint;
						break;
					case IT::CHAR:
					case IT::BOOL:
						*file << EQU << idtable->table[i + 1].value.vchar;
						break;
					case IT::STR:
						*file << EQU << idtable->table[i + 1].value.vstr->str;
					}
					idtable->table[i].iddatatype = IT::NUL;
				}
				else
					throw ERROR_THROW_LINE(138, lextable->table[idtable->table[i].idxfirstLE].sn);
				*file << ENDL;
				
			}
		}
		*file << ENDL;
	}
	void PrintVariables(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		*file << COMMENTVAR;
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
						*file << BYTE;
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
						*file << DWORD;
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
					case IT::BOOL:
					case IT::CHAR:
					{
						temp += idtable->table[lextable->table[i].idxTI].id;
						temp += UNDERLINE;
						temp += idtable->table[lextable->table[i].idxTI].prefix;
						file->setf(std::ios::left);
						file->width(24);
						*file << temp;
						file->width(10);
						*file << BYTE;
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
					if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::C) {
						if (lextable->table[poslenta - 1].lexema[0] != LEX_TYPE && lextable->table[poslenta + 1].lexema[0] == LEX_ASSIGN)
							throw ERROR_THROW_LINE(139, lextable->table[poslenta].sn);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A && lextable->table[poslenta - 1].lexema[0] == LEX_TYPE)
					{
						poslenta = FillArray(lextable, idtable, poslenta);
					}
					else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A) {
						poslenta = ArrayFillIndex(lextable, idtable, poslenta);
					}
					else {
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, true, false, true);
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
					poslenta = PrintÑondition(lextable, idtable, poslenta, true);
					*file << "@f" << (CountCondition-1) << ":" << ENDL;
					break;
				}
				case LEX_ELSE:
				{
					*file << "jmp @t" << CountCondition << ENDL;
					poslenta = PrintÑondition(lextable, idtable, poslenta+1, false);		//if else
					break;
				}
				}

				poslenta++;
			} while (countbraces);
			*file << ENDL;
			*file << ERROR_ASM;
			if (lextable->table[idtable->table[positionnFunc].idxfirstLE].lexema[0] == LEX_MAIN)
				*file << EXIT;
			else
				*file << RET;
			*file << idtable->table[positionnFunc].id << SPACE << ENDP;
			*file << std::endl;
		}
#pragma endregion
		*file << WRITE<<ENDL;
		*file << WRITELINE << ENDL;
		*file << INT_TO_CHAR <<ENDL;
		*file << ENDMAIN;
		posfunc.clear();
	}
	int  ProcessingDeclaration(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta, bool recursion, bool assign, bool firstcall)
	{
		
		PrintChain(lextable, idtable, poslenta - 1);
		while (lextable->table[poslenta].idxTI == TI_NULLIDX) poslenta++;
		int posID = lextable->table[poslenta].idxTI;
		if (firstcall) poslenta++;
		char ID[32];
		bool registers16 = true, registers32 = true, registersFPU = true;
		REGISTERS  reg32 = eax;
		int stack = 0;
		int static count = 0;

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
					//if (!firstcall) {
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
						*file << MOV_ECX_ADDR << idtable->table[lextable->table[poslenta].idxTI].id;	    // àäðåññ ìàññèâà asm
						*file << UNDERLINE;
						*file << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
						*file << PUSH_ECX;
						 ProcessingDeclaration(lextable, idtable, poslenta+1, true, false, false);					// ðåêóðñèÿ 
						*file << POP_ECX;
						*file << IMUL_EAX_TYPE << idtable->table[lextable->table[posarray].idxTI].id;			// âû÷åñëèòü ðàçìåð â áàéòàõ äëÿ ñìåùåíèÿ																// âîññòàíàâëèâàåì ðåãèñòð â êîòîðîì ñîäåðæèòñÿ ðåçóëüòàò
						*file << UNDERLINE;
						*file << idtable->table[lextable->table[posarray].idxTI].prefix << ENDL;
						*file << ADD_ECX_EAX;
						*file << MOV_EDX << PREFERRED_ECX;
					
					switch (reg32) {
					case ebx:
					{
						*file << POP_EAX;
						*file << MOV_EBX << EDX << ENDL;
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
					}
					//return poslenta;
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
						temp = LITERALS ;
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
						temp = LITERALS ;
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
						temp = LITERALS ;
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
						temp = LITERALS ;
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
						temp = LITERALS ;
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
						temp = LITERALS ;
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
						if(warning_lvl) *file << OVERFLOW_ASM;
						break;
					}
					case ecx:
					{
						if (stack > 1)
						{
							*file << POP_ECX << POP_EDX;
							*file << ADD_ECX_EDX;
							if(warning_lvl) *file << OVERFLOW_ASM;
							*file << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							*file << POP_ECX;
							*file << ADD_EBX_ECX;
							if(warning_lvl) *file << OVERFLOW_ASM;
							stack--;
							reg32 = ebx;
						}
						else {
							*file << ADD_EAX_EBX;
							if(warning_lvl) *file << OVERFLOW_ASM;
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
						if(warning_lvl) *file << OVERFLOW_ASM;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							*file << POP_ECX << POP_EDX;
							*file << SUB_ECX_EDX;
							if(warning_lvl) *file << OVERFLOW_ASM;
							*file << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							*file << POP_ECX;
							*file << SUB_EBX_ECX;
							if(warning_lvl) *file << OVERFLOW_ASM;
							stack--;
							reg32 = ebx;
						}
						else {
							*file << SUB_EAX_EBX;
							if(warning_lvl) *file << OVERFLOW_ASM;
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
						*file << XOR_EDX;
						*file << CMP_EBX_0;
						*file << DIV_EAX_EBX;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							*file << POP_ECX << POP_EDX;
							*file << XOR_EDX;
							*file << CMP_EBX_0;
							*file << DIV_ECX_EDX;
							*file << PUSH_ECX;
							stack--;
						}
						else if (stack == 1)
						{
							*file << POP_ECX;
							*file << XOR_EDX;
							*file << CMP_EBX_0;
							*file << DIV_EBX_ECX;
							stack--;
							reg32 = ebx;
						}
						else {
							*file << XOR_EDX;
							*file << CMP_EBX_0;
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
						if (warning_lvl) *file << OVERFLOW_ASM;
						break;
					}
					case ecx:
					{

						if (stack > 1)
						{
							if (warning_lvl) 
							{
								*file << POP_ECX << POP_EDX;
								*file << PUSH_EAX;
								*file << PUSH_EBX;
								*file << MOV_EAX_ECX;
								*file << MOV_EBX_EDX;

								*file << MUL_EAX_EBX;
								*file << OVERFLOW_ASM;
								*file << MOV_ECX_EAX;
								*file << POP_EBX;
								*file << POP_EAX;
								*file << PUSH_ECX;
							}
							else {
								*file << POP_ECX << POP_EDX;
								*file << MUL_ECX_EDX;
								*file << PUSH_ECX;
							}
							stack--;
						}
						else if (stack == 1)
						{
							if (warning_lvl)
							{
								*file << POP_ECX;
								*file << PUSH_EAX;
								*file << MOV_EAX_EBX;
								*file << XOR_EDX;
								*file << DIV_EAX_ECX;
								*file << MOV_EBX_EAX;
							}
							else 
							{
								*file << POP_ECX;
								*file << MUL_EBX_ECX;
								if (warning_lvl) *file << OVERFLOW_ASM;
							}
							stack--;
							reg32 = ebx;
						}
						else {
							*file << MUL_EAX_EBX;
							if (warning_lvl) *file << OVERFLOW_ASM;
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
						if (warning_lvl) *file << OVERFLOW_ASM_BYTE;
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
				}
			*file << ENDL;
			}

			case LEX_SQUARE_RIGHT:
			case LEX_COMMA:
			case LOGICALS:
			{
				return poslenta;
			}
			}
			poslenta++;
			
		}
		return poslenta;
	}
	int  FillArray(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		bool start = false;
		bool first = false;
		int count = 0;
		char ID[32];
		PrintChain(lextable, idtable, poslenta - 1);
		*file << XOR_EAX;
		*file << MOV_EBX_TYPE << idtable->table[lextable->table[poslenta].idxTI].id;
		*file << UNDERLINE;
		*file << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
		*file << MOV_ECX_ADDR << idtable->table[lextable->table[poslenta].idxTI].id;
		*file << UNDERLINE;
		*file << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL << ENDL;

		std::string temp;
		while (lextable->table[poslenta++].lexema[0] != LEX_SEMICOLON)
		{
			if (lextable->table[poslenta].lexema[0] == LEX_LEFTBRACE) start = true;
			if (start)
			{	if(lextable->table[poslenta].idxTI!=TI_NULLIDX)
				if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::L) {
					*file << ";" << idtable->table[lextable->table[poslenta].idxTI].id << "[" << count << "]" << "=" << idtable->table[lextable->table[poslenta].idxTI].value.vint << ENDL;
					if(first) *file << ADD_EAX_EBX;
					temp = LITERALS ;
					temp += UNDERLINE;
					temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
					*file << MOV_EDX << temp << ENDL;
					*file << MOV_ECX_EAX_EDX;
					*file << ENDL;
					first = true;
					count++;
				}

			}
		}
		return poslenta - 1;

	}
	int  ArrayFillIndex(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta) {
		PrintChain(lextable, idtable, poslenta - 1);
		int posID = poslenta;
		while (lextable->table[poslenta].lexema[0] != LEX_SEMICOLON)
		{
			if(lextable->table[poslenta++].lexema[0] == LEX_ASSIGN)
				poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, false, false);
		}
		*file << PUSH_EAX;
		*file << MOV_ECX_ADDR << idtable->table[lextable->table[posID].idxTI].id;
		*file << UNDERLINE;
		*file << idtable->table[lextable->table[posID].idxTI].prefix << ENDL;
		*file << MOV_EBX_TYPE << idtable->table[lextable->table[posID].idxTI].id;	   
		*file << UNDERLINE;
		*file << idtable->table[lextable->table[posID].idxTI].prefix << ENDL;
		*file << PUSH_EBX;
		ProcessingDeclaration(lextable, idtable, posID, false, true, true);
		*file << POP_EBX;
		*file << MUL_EAX_EBX;
		*file << ADD_ECX_EAX;
		*file << POP_EAX;
		*file << MOV <<PREFERRED_ECX_C << EAX << ENDL << ENDL;

		return poslenta-1;
	}
	int PrintWriteFunctions(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		int posID = lextable->table[poslenta].idxTI;
		bool atLeatsOneParametres = false;
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
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, true, false);
						*file << "invoke int_to_char, ADDR output, eax\n";// << LITERALS  << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;

						if (idtable->table[posID].value.vbool)
							*file << "invoke write, ADDR output\n";
						else
							*file << "invoke writeline, ADDR output\n";
						break;
					case IT::FLOAT:
						break;
					case IT::STR:

						if (idtable->table[posID].value.vbool)
							*file << "invoke write, ADDR ";
						else
							*file << "invoke writeline, ADDR ";
						*file << LITERALS  << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;
						break;
					}
					break;
				case IT::V:
					switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
					{
					case IT::LONG:
					case IT::CHAR:
						*file << "invoke int_to_char, ADDR output," 
							<< idtable->table[lextable->table[poslenta].idxTI].id 
							<< UNDERLINE << idtable->table[lextable->table[poslenta].idxTI].prefix 
							<< ENDL;						
						if (idtable->table[posID].value.vbool)
							*file << "invoke write, ADDR output\n";
						else
							*file << "invoke writeline, ADDR output\n";
						break;
					case IT::FLOAT:
						break;
					case IT::STR:
						break;
					}

					break;
				case IT::A:
					int posfordatatype = poslenta;
					poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, true, false);
					*file << "invoke int_to_char, ADDR output";
					switch (idtable->table[lextable->table[posfordatatype].idxTI].iddatatype)
					{
					case IT::LONG:
						*file << EAX << ENDL;
						break;
					case IT::CHAR:
						*file << AL << ENDL;
						break;
					}
					if (idtable->table[posID].value.vbool)
						*file << "invoke write, ADDR output\n";
					else
						*file << "invoke writeline, ADDR output\n";
					*file << ENDL;
				}
			}

		}
		if(!atLeatsOneParametres) *file << "invoke write, ADDR endl\n"; // not at leats one parametres
		return poslenta - 1;
	}
	int PrintÑondition(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta, bool lexif)
	{
		int countbraces = 0;
		bool elseis = false;
		if (lexif)
		{
			bool eax = true;
			int poslogical = 0;
			while (lextable->table[poslenta].lexema[0] != LEX_LEFTBRACE) {
				if (lextable->table[poslenta].lexema[0] == LOGICALS)
					poslogical = poslenta;
				if (lextable->table[poslenta].idxTI != TI_NULLIDX) {
					switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
					{
					case IT::L:
						switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
						{
						case IT::LONG:
							if (eax)	*file << MOV_EAX;
							else		*file << MOV_EBX;
							*file << LITERALS << UNDERLINE << lextable->table[poslenta].idxTI << ENDL;
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
							if (eax) *file << MOV_EAX;
							else	 *file << MOV_EBX;
							*file << idtable->table[lextable->table[poslenta].idxTI].id;
							*file << UNDERLINE;
							*file << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
							break;
						case IT::CHAR:
						case IT::BOOL:
							if (eax)*file << MOV_AL;
							else	*file << MOV_BL;
							*file << idtable->table[lextable->table[poslenta].idxTI].id;
							*file << UNDERLINE;
							*file << idtable->table[lextable->table[poslenta].idxTI].prefix << ENDL;
							break;
						case IT::STR:
							throw ERROR_THROW_LINE(140, lextable->table[poslenta].sn);
							break;
						}
					case IT::A:
						if (!eax) *file << PUSH_EAX << ENDL;
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta, false, true, false) - 1;
						if (!eax) {
							*file << POP_EBX;
						}
						break;

					}
					eax = false;
				}
				poslenta++;
			}

			*file << CMP_EAX_EBX;
			switch (lextable->table[poslogical].expression[0])
			{
			case '<':
			{	if (lextable->table[poslogical].expression[1])
				*file << "jbe @f" << CountCondition << "\t;	<=\n";		// <=
			else
				*file << "jle @f" << CountCondition << "\t;	<\n";		// <
			break;
			}
			case '>':
			{
				if (lextable->table[poslogical].expression[1])
					*file << "jl @f" << CountCondition << "\t; >=\n";	// >=
				else
					*file << "jge @f" << CountCondition << "\t; >\n";	// >
				break;
			}
			case '=':
			{
				*file << "jne @f" << CountCondition << "\t; ==\n";		// ==
				break;
			}
			case '!':
			{
				*file << "je @f" << CountCondition << "\t; !=\n";// !=
				break;
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
				else if (idtable->table[lextable->table[poslenta].idxTI].idtype == IT::A) {
					poslenta = ArrayFillIndex(lextable, idtable, poslenta);
				}
				else {
					poslenta = ProcessingDeclaration(lextable, idtable, poslenta, true, false, true);
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

				poslenta = PrintÑondition(lextable, idtable, poslenta,true);
				break;
			}
			case LEX_ELSE:
			{

				poslenta = PrintÑondition(lextable, idtable, poslenta, false);		//if else
				break;
			}
			}

			poslenta++;
		} while (countbraces);
		if(!lexif)
			*file << "@t" << CountCondition << ":" << ENDL;
		CountCondition++;
		return poslenta - 1;
	}
	int PrintCyclesint(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		return 0;
	}
	void PrintChain(LT::LexTable* lextable, IT::IdTable* idtable, int pos)
	{
		int poslenta = pos;
		*file << SEMICOLON;
		*file << LEFTHESIS << lextable->table[poslenta].sn << RIGHTESIS;
		while (lextable->table[poslenta++].lexema[0] != SEMICOLON)
		{
			if (lextable->table[poslenta++].lexema[0] != LEX_LEFTBRACE)
				break;
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


