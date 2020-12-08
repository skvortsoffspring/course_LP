#include "stdafx.h"

namespace GTA 
{

	void GeneratorASM(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		char n = '\n';
		std::ofstream* file = new std::ofstream;
		file->open(FILE, std::ios::out);
		if (file->fail())
			throw ERROR_THROW(115);
		PrintDataTime(file);

		*file << HEAD_ASM;
		*file << LIBRARY;
		*file << PROTO;
		//PrintProto(lextable, idtable);
	}
	void PrintDataTime(std::ofstream *file) 
	{
		char buffer[48];
		time_t rawtime;
		struct tm* timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 48, "%d-%m-%Y --%A-- %H:%M:%S ", timeinfo);
		*file <<";--" << buffer << "--\n";
	}
	void PrintLiteral(LT::LexTable* LexTable, IT::IdTable* IdTable)
	{

	}
	void PrintProto(LT::LexTable* lextable, IT::IdTable* idtable)
	{
		for (int i = 0; i < lextable->size; i++) 
		{
			if (lextable->table[i].lexema[0] == LEX_LITERAL) {
				switch (idtable->table[lextable->table[i].idxTI].iddatatype) {
				case IT::LONG:
				{
					std::cout.width(10); std::cout << "long";
						std::cout.width(10); std::cout << idtable->table[lextable->table[i].idxTI].value.vint;

					break;
				}
				case IT::FLOAT:
				{
					std::cout.width(10); std::cout << "float";

						std::cout.width(10); std::cout << idtable->table[lextable->table[i].idxTI].value.vfloat;

					break;
				}
				case IT::STR:
				{
					std::cout.width(10); std::cout << "string ";

						std::cout.width(10); std::cout << idtable->table[lextable->table[i].idxTI].value.vstr;
	
					break;
				}
				
				}
				std::cout << std::endl;
			}
		}
	}
}