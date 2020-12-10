#include "stdafx.h"

namespace GTA 
{
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
		*file <<";--" << buffer << "--\n";
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
					temp+= itoa(lextable->table[i].idxTI,buffer,10);
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
	void PrintProtos(LT::LexTable* lextable, IT::IdTable* idtable, std::vector<int> &posfunc)
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
				index < lextable->table[i].idxTI &&
				idtable->table[lextable->table[i].idxTI].idtype == IT::V)
			{
				std::string temp = "";
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
					*file << TI_STR_MAXSIZE; //itoa(idtable->table[lextable->table[i].idxTI].value.vstr->len, buffer, 10);	//TI_STR_MAXSIZE;
					*file << " DUP(0)";
					*file << std::endl;
					break;
				}
				}
				index = lextable->table[i].idxTI;
			}
		}
	}
	void PrintFunctions(LT::LexTable* lextable, IT::IdTable* idtable, std::vector<int> &posfunc)
	{
		*file << COMMENTCODE;
		*file << CODE;

		auto iter = posfunc.cbegin();
		bool findedmain = false;
		int countbraces = 0;
		int poslenta = 0;															// print functions and parametre id and type
		int positionnFunc = 0;
		for (int i = 0,p = 0 ; i < posfunc.size(); i++)
		{
			p = posfunc[i];
#pragma region findedmain
			if (lextable->table[idtable->table[p].idxfirstLE].lexema[0] == LEX_MAIN)
			{
				findedmain = true;
				posfunc.erase(iter + i);
				i = -1;
			}
#pragma endregion
#pragma region printID&Parametres
			if (idtable->table[p].idtype == IT::F && findedmain)
			{
				positionnFunc = p;
				file->setf(std::ios::left);
				*file << idtable->table[p].id;
				*file << SPACE;
				*file << PROC;
				*file << SPACE;

				if(idtable->table[p+1].idtype == IT::P)
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
					if(idtable->table[p+1].idtype == IT::P)
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
			if (findedmain) {
				
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
						poslenta = ProcessingDeclaration(lextable, idtable, poslenta);
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
		}
#pragma endregion
		*file << ENDMAIN;
		posfunc.clear();
	}
	int ProcessingDeclaration(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		PrintChain(lextable, idtable, poslenta-1);
		int posID = lextable->table[poslenta].idxTI;
		char ID[32];

		bool registers16 = true, registers32 = true, registersFPU = true; // проверка потери данных
		bool eax = true;
		bool ebx = true;
		bool al = true;
		bool bl = true;
		while (lextable->table[poslenta++].lexema[0] != LEX_SEMICOLON) {
			std::string temp;
			switch (lextable->table[poslenta].lexema[0])
			{
				case LEX_ID:
				{
					switch (idtable->table[lextable->table[poslenta].idxTI].idtype)
					{
						case IT::V:
						{
							switch (idtable->table[lextable->table[poslenta].idxTI].iddatatype)
							{
								case IT::LONG:
								{
									if (eax) {
										*file << MOV_EAX;
										temp += idtable->table[lextable->table[poslenta].idxTI].id;
										temp += UNDERLINE;
										temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
										*file << temp;
										*file << ENDL;
										eax = false;
									}
									else
									{
										*file << MOV_EBX;
										temp += idtable->table[lextable->table[poslenta].idxTI].id;
										temp += UNDERLINE;
										temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
										*file << temp;
										*file << ENDL;
										ebx = false;
									}
									
									break;
								}
								case IT::BOOL:
								case IT::CHAR:
								{
									if (bl) {
										*file << MOV_BL;
										temp += idtable->table[lextable->table[poslenta].idxTI].id;
										temp += UNDERLINE;
										temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
										*file << temp;
										*file << ENDL;
										bl = false;
									}
									else
									{
										*file << MOV_AL;
										temp += idtable->table[lextable->table[poslenta].idxTI].id;
										temp += UNDERLINE;
										temp += idtable->table[lextable->table[poslenta].idxTI].prefix;
										*file << temp;
										*file << ENDL;
										al = false;
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
						case IT::A:
						{
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
							if (eax){
								*file << MOV_EAX; 
								temp = "literal";
								temp += UNDERLINE;
								temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
								*file << temp;
								*file << ENDL;
								eax = false;
							}
							else
							{
								*file << MOV_EBX;
								temp = "literal";
								temp += UNDERLINE;
								temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
								*file << temp;
								*file << ENDL;
								ebx = false;
							}
							break;
						}
						case IT::CHAR:
						case IT::BOOL:
						{
							*file << MOV_BL;
							temp = "literal";
							temp += UNDERLINE;
							temp += itoa(lextable->table[poslenta].idxTI, ID, 10);
							*file << temp;
							*file << ENDL;
							bl = false;
							break;
						}
						case IT::FLOAT:
						{
							break;
						}
					}
				}
				case EXPRESSIONS:
					switch (lextable->table[poslenta].expression[0])
					{
					case ADD:
						if (!eax && !ebx || !eax && !bl)
						{
							*file << ADD_EAX_ABX;
							ebx = true; bl = true;
						}
						break;
					case SUB:
						if (!eax && !ebx || !eax && !bl)
						{
							*file << SUB_EAX_ABX;
							ebx = true; bl = true;
						}
						break;
					case MUL:
						if (!eax && !ebx || !eax && !bl)
						{
							*file << MUL_EAX_ABX;
							ebx = true; bl = true;
						}
					case DIV:
						if (!eax && !ebx || !eax && !bl)
						{
							*file << DIV_EAX_ABX;
							ebx = true; bl = true;
						}
						break;
					}
					break;
				case LEX_SEMICOLON:
					if (!eax){
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
					}
			}
		}
		return poslenta-1;
	}
	void PrintChain(LT::LexTable* lextable, IT::IdTable* idtable, int poslenta)
	{
		*file << SEMICOLON;
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
