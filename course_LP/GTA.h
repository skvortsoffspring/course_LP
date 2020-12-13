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
#define PUSH_EAX	"push eax\n"
#define PUSH_EBX	"push ebx\n"
#define PUSH_ECX	"push ecx\n"
#define PUSH_EDX	"push edx\n"
#define POP			"pop "
#define POP_EAX		"pop eax\n"
#define POP_EBX		"pop ebx\n"
#define POP_ECX		"pop ecx\n"
#define POP_EDX		"pop edx\n"

// mov
#define MOV			"mov "
#define MOV_EAX		"mov eax, "
#define MOV_EBX		"mov ebx, "
#define MOV_ECX		"mov ecx, "
#define MOV_EDX		"mov edx, "
#define MOV_AL		"mov al, "
#define MOV_BL		"mov bl, "
#define MOV_CL		"mov cl, "
#define MOV_DL		"mov dl, "

//operations long
#define ADD_EAX_EBX "add eax, ebx\n"	// сложить
#define ADD_EBX_ECX "add ebx, ecx\n"	// сложить
#define ADD_ECX_EDX "add ecx, edx\n"	// сложить

#define SUB_EAX_EBX "sub eax, ebx\n"	// вычесть
#define SUB_EBX_ECX "sub ebx, ecx\n"	// вычесть
#define SUB_ECX_EDX "sub ecx, edx\n"	// вычесть

#define DIV_EAX_EBX "idiv eax, ebx\n"	// делелить
#define DIV_EBX_ECX "idiv ebx, ecx\n"	// делелить
#define DIV_ECX_EDX "idiv ecx, edx\n"	// делелить

#define MUL_EAX_EBX "imul eax, ebx\n"	// умножить
#define MUL_EBX_ECX "imul ebx, ecx\n"	// умножить
#define MUL_ECX_EDX "imul ecx, edx\n"	// умножить

//operations byte
#define ADD_AL_BL	"add al, bl\n"		// сложить
#define SUB_AL_BL	"sub al, bl\n"		// вычесть
#define DIV_AL_BL	"idiv al, bl\n"		// делить
#define MUL_AL_BL	"imul al, bl\n"		// умножить

// array 
#define MOV_ECX_ADDR	"mov ecx, offset "
#define IMUL_EAX_TYPE	"imul eax, type "
#define	MOV_EDX_EAX		"mov edx, eax\n"
#define ADD_ECX_EAX		"add ecx, eax\n"
#define	MOV_EAX_EDX		"mov eax, edx\n"
#define ADD_EAX_EDX		"add eax, edx\n"
#define PTR				"ptr "
#define PREFERRED_ECX	"[ecx]\n"

#define ENDMAIN "end main"
#pragma endregion
#pragma region typeDATA
#define BYTE	"BYTE"
#define DWORD	"DWORD"
#define REAL4	"REAL4"
#define DEFREAL "0.0"
#define DUP0	" DUP(0)"
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
	int ProcessingDeclaration(LT::LexTable*, IT::IdTable*, int, bool);
	void PrintChain(LT::LexTable*, IT::IdTable*, int);
	int ProcessingArray(LT::LexTable*, IT::IdTable*, int);
}