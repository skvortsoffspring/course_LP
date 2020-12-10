#pragma once
#include <vector>
#define FILE			"source.asm"								// file for generation
#define COMMENTLITERAL	";------------- LITERALS -------------\n"
#define COMMENTPROTO	";------------ PPOTO USER ------------\n"
#define COMMENTCODE		";--------------- CODE ---------------\n"
#define COMMENTVAR		";------------- VARIABLES ------------\n"
#define CONST			";--------------- CONST --------------\n"	\
						".CONST\n"									\
						"str_end		EQU		0\n"				\
						"endl		dword	0Ah, 0\n"				\
						"\n"
#define DATA	".DATA\n"
#define CODE	".CODE\n"
#define LIBRARY "includelib kernel32.lib\n"							\
				"includelib libucrt.lib\n\n"										

#define HEAD_ASM ";-------------ASM-------------\n"					\
				  ".586p\n"											\
				  ".model flat, stdcall\n"							\
				  ".stack 4096\n"									\
				  "\n"
#define PROTO   ";---------- STANDART PROTO ----------\n"\
				"WriteConsoleA			PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD\n"\
				"MessageBoxA				PROTO : DWORD, : DWORD, : DWORD, : DWORD\n"\
				"write					PROTO : DWORD, : DWORD\n"\
				"writeline				PROTO : DWORD, : DWORD\n"\
				"int_to_char				PROTO : DWORD, : DWORD\n"\
				"GetStdHandle			PROTO : DWORD\n"\
				"SetConsoleOutputCP		PROTO : DWORD\n"\
				"SetConsoleCP			PROTO : DWORD\n"\
				"ExitProcess				PROTO : DWORD\n\n"

#pragma region shortTEXT
#define SPP ", : "		// separator parametres prototypes
#define SFP " : "		// separator functions parametres
#define UNDERLINE "_"
#pragma endregion


#pragma region commandsASM
// standarts
#define PROC		"PROC"
#define ENDP		"ENDP\n"
#define START		"START :\n"
#define RET			"ret\n"
// registers
#define EAX			", eax"
#define AL			", al"
// stack
#define PUSH		"push "
#define POP			"pop "

// mov
#define MOV			"mov "
#define MOV_EAX		"mov eax, "
#define MOV_EBX		"mov ebx, "
#define MOV_AL		"mov al, "
#define MOV_BL		"mov bl, "

//operations long
#define ADD_EAX_ABX "add eax, ebx\n"	// сложить
#define SUB_EAX_ABX "sub eax, ebx\n"	// вычесть
#define DIV_EAX_ABX "div ebx\n"			// делелить
#define MUL_EAX_ABX "mul ebx\n"			// умножить
//operations byte
#define ADD_AL_BL	"add al, bl\n"		// сложить
#define SUB_AL_BL	"sub al, bl\n"		// вычесть
#define DIV_AL_BL	"div bl\n"			// делить
#define MUL_AL_BL	"mul bl\n"			// умножить

#define ENDMAIN "end main"
#pragma endregion
#pragma region typeDATA
#define BYTE	"BYTE"
#define DWORD	"DWORD"
#define REAL4	"REAL4"
#define DEFREAL "0.0"
#pragma endregion


namespace GTA 
{
	void GeneratorASM(LT::LexTable*, IT::IdTable*);
	void FindAll(std::vector<int>&, std::vector<int>&, std::vector<int>&);
	void PrintDataTime();
	void PrintLiterals(LT::LexTable*, IT::IdTable*);
	void PrintProtos(LT::LexTable*, IT::IdTable*, std::vector<int>&);
	void PrintFunctions(LT::LexTable*, IT::IdTable*, std::vector<int>&);
	void PrintVariables(LT::LexTable*, IT::IdTable*);
	int ProcessingDeclaration(LT::LexTable*, IT::IdTable*, int);
	void PrintChain(LT::LexTable*, IT::IdTable*, int);
}