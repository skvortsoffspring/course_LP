#pragma once
#define FILE "source.asm"									// file for generation

#define LIBRARY "includelib kernel32.lib\n"			\
				"includelib libucrt.lib\n\n"				

#define HEAD_ASM ";-------------ASM-------------\n"	\
				  ".686p\n.XMM\n"					\
				  ".model flat, C\n"				\
				  ".stack 4096\n\n"
#define PROTO   ";---------- STANDART PROTO ----------\n"\
				"WriteConsoleA			PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD\n"\
				"MessageBoxA				PROTO : DWORD, : DWORD, : DWORD, : DWORD\n"\
				"write					PROTO : DWORD, : DWORD\n"\
				"writeline				PROTO : DWORD, : DWORD\n"\
				"int_to_char				PROTO : DWORD, : DWORD\n"\
				"GetStdHandle			PROTO : DWORD\n"\
				"SetConsoleOutputCP		PROTO : DWORD\n"\
				"SetConsoleCP			PROTO : DWORD\n"\
				"ExitProcess				PROTO : DWORD"

namespace GTA 
{
	void GeneratorASM(LT::LexTable*, IT::IdTable*);
	void PrintDataTime(std::ofstream*);
	void PrintLiteral(LT::LexTable*, IT::IdTable*);
	void PrintProto(LT::LexTable*, IT::IdTable*);
}