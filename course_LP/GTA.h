#pragma once
#include <vector>
#define FILE			*file										// file for generation
#define COMMENTLITERAL	";------------- LITERALS -------------;\n"
#define COMMENTPROTO	";------------ PPOTO USER ------------;\n"
#define COMMENTCODE		";--------------- CODE ---------------;\n"
#define COMMENTVAR		";------------- VARIABLES ------------;\n"
#define USERCONST		";-------------USER CONST ------------;\n"	
#define CONST			";--------------- CONST --------------;\n"	\
						".CONST\n"									\
						"str_end				EQU		0\n"		\
						"endl				dword	0Ah, 0\n"		\
						"\n"
#define SYSTEMERRORS	";--------------- SYSTEM ERROR ---------------;\n"	\
						"errorid1				DWORD	  1\n"				\
						"errorid2				DWORD	  2\n"				\
						"errorid3				DWORD	  3\n"				\
						"errorid4				DWORD	  4\n"				\

#define DATA	".DATA\n"													\
				";------------- SYSTEM VARIABLE -------------;\n"			\
				"output					BYTE	 255 DUP(0)\n"				

#define CODE	".CODE\n"

#define LIBRARY "includelib kernel32.lib\n"									\
				"includelib libucrt.lib\n"									\
				"includelib user32.lib\n"									\
				"includelib ucrt.lib\n\n"										\
				"includelib ../Debug/string.lib\n"							\
				"includelib ../Debug/std.lib\n"								\
				"includelib ../Debug/error.lib\n"							\
				"includelib ../Debug/random.lib\n"							

#define HEAD_ASM ";-------------ASM-------------;\n"						\
				  ".586p\n"													\
				  ".model flat, stdcall\n"									\
				  ".stack 4096\n"											\
				  "\n"
#define PROTO   ";---------- STANDART PROTO ----------;\n"					\
				"int_to_char  			PROTO : DWORD, : DWORD\n"			\
				"strcat		   			PROTO : DWORD, : DWORD\n"			\
				"strcopy\t			   	PROTO : DWORD, : DWORD\n"			\
				"write					PROTO : DWORD\n"					\
				"writeline				PROTO : DWORD\n"					\
				"GetStdHandle			PROTO : DWORD\n"					\
				"SetConsoleOutputCP		PROTO : DWORD\n"					\
				"strlen			 		PROTO : DWORD\n"					\
				"SetConsoleCP			PROTO : DWORD\n"					\
				"@error					PROTO : DWORD\n"					\
				"random			 		PROTO : DWORD, : DWORD\n"			\
				"ExitProcess  			PROTO : DWORD\n\n"

#define EXIT	"\tpush 0\n"												\
				"\tcall ExitProcess\n"

#define ERROR_ASM "\tjmp OK\n"												\
				"\terrordivzero:\n"											\
				"\tinvoke @error, errorid1\n"								\
				"\toverflow:\n"												\
				"\tinvoke @error, errorid2\n"								\
				"\toverflowbyte:\n"											\
				"\tinvoke @error, errorid4\n"								\
				"OK:\n"
#define DEVISION_OVERFLOW	";--- division overflow ---;\n"
	
//invoke int_to_char, ADDR output, [ecx]
//invoke write, ADDR output
#pragma region shortTEXT
#define SPP				", : "		// separator parametres prototypes
#define SFP				" : "		// separator functions parametres
#define COLON			":"
#define UNDERLINE		"_"
#define ASSIGN			'='
#define LEFTSQUARE		'['
#define RIGHTSQUARE		']'
#define STR_END			"str_end\n"
#define LITERALS		"literal"
#define JUMP_TO_T		"jmp @t"
#define JBE_TO_F		"jbe @f"	// <=
#define JLE_TO_F		"jle @f"	// <
#define JL_TO_F			"jl @f"		// >=
#define JGE_TO_F		"jge @f"	// >
#define JNE_TO_F		"jne @f"	// ==
#define JE_TO_F			"je @f"		// !=
#define LABEL_F			"@f"
#define LABEL_T			"@t"

#pragma endregion


#pragma region commandsASM
// standarts
#define PROC			"PROC"
#define ENDP			"ENDP\n"
#define START			"START :\n"
#define RET				"ret\n"
#define EQU				"EQU"
#define CALL			"call "
#define PUSH_OFFSET		"\tpush offset "
#define PUSH_LENGHTOFF "\tpush lengthof "
#define CLD				"\tcld\n"
#define LEA_EDI			"\tlea edi, "
#define REP_MOVSB		"\trep movsb\n"
#define OFFSET			"offset "
// errors macroses
#define OVERFLOW_ASM		"\tjc overflow\n"

#define OVERFLOW_ASM_BYTE	"\tcmp eax, 0100h\n"\
							"\tjge overflowbyte\n"
// increment decrement
#define INCREMENT	"\tinc "
#define DECREMENT	"\tdec "

// registers		
#define EAX			", eax"
#define AL			", al"
#define EDX			"edx"
#define DL			", dl"
// stack			 
#define PUSH		"\tpush "
#define PUSH_EAX	"\tpush eax\n"
#define PUSH_EBX	"\tpush ebx\n"
#define PUSH_ECX	"\tpush ecx\n"
#define PUSH_EDX	"\tpush edx\n"
#define POP			"\tpop "
#define POP_EAX		"\tpop eax\n"
#define POP_EBX		"\tpop ebx\n"
#define POP_ECX		"\tpop ecx\n"
#define POP_EDX		"\tpop edx\n"
#define POP_ESI		"\tpop esi\n"
					
// mov				 
#define MOV			"\tmov "
#define MOV_EAX		"\tmov eax, "
#define MOV_EBX		"\tmov ebx, "
#define MOV_ECX		"\tmov ecx, "
#define MOV_EDX		"\tmov edx, "
#define MOV_AL		"\tmov al, "
#define MOV_BL		"\tmov bl, "
#define MOV_CL		"\tmov cl, "
#define MOV_DL		"\tmov dl, "
#define MOV_BL_AL	"\tmov bl, al\n"

//operations long
#define ADD_EAX_EBX "\tadd eax, ebx\n"	// сложить
#define ADD_EBX_ECX "\tadd ebx, ecx\n"	// сложить
#define ADD_ECX_EDX "\tadd ecx, edx\n"	// сложить
					 
#define SUB_EAX_EBX "\tsub eax, ebx\n"	// вычесть
#define SUB_EBX_ECX "\tsub ebx, ecx\n"	// вычесть
#define SUB_ECX_EDX "\tsub ecx, edx\n"	// вычесть
					 
#define DIV_EAX_EDX "\tidiv edx\n"		// делелить
#define DIV_EAX_ECX "\tidiv ecx\n"		// делелить
#define DIV_EAX_EBX "\tidiv ebx\n"		// делелить
#define DIV_EAX_ECX	"\tidiv ecx\n"		// делелить
					 
#define MUL_EAX_EBX "\timul ebx\n"		// умножить
#define MUL_EBX_ECX "\timul ebx, ecx\n"	// умножить
#define MUL_ECX_EDX "\timul ecx, edx\n"	// умножить
#define MUL_EAX_EDX "\timul eax, edx\n"	// умножить
#define MUL_EAX_ECX "\timul eax, ecx\n"	// умножить
#define MUL_ECX		"\timul ecx,"			// умножить
#define MUL_EAX		"\timul eax,"			// умножить

//operations byte
#define ADD_AL_BL	"\tadd al, bl\n"		// сложить
#define SUB_AL_BL	"\tsub al, bl\n"		// вычесть
#define DIV_AL_BL	"\tidiv al, bl\n"		// делить
#define MUL_AL_BL	"\timul al, bl\n"		// умножить
#define ADD_ECX		"\timul ecx, \n"		// умножить
#define DIV_EDX		"\timul edx\n";
#define DIV_ECX		"\timul ecx\n";

// logic
#define XOR_EAX			"\txor eax, eax\n"
#define XOR_EBX			"\txor ebx, ebx\n"
#define XOR_ECX			"\txor ecx, ecx\n"
#define XOR_EDX			"\txor edx, edx\n"
// cmp					 
#define CMP_EAX_EBX		"\tcmp eax, ebx	\n"
#define CMP_EBX_ECX		"\tcmp ebx, ecx	\n"
#define CMP_ECX_EDX		"\tcmp ecx, edx	\n"
#define CMP_EAX_EDX		"\tcmp eax, edx	\n"
// array 				 
#define MOV_ECX_ADDR	"\tmov ecx, offset "
#define MOV_EAX_ADDR	"\tmov ecx, offset "
#define MOV_EDX_ADDR	"\tmov edx, offset "
						 
#define MOV_EBX_TYPE	"\tmov ebx, type "
#define IMUL_EAX_TYPE	"\timul eax, type "
#define MOV_ECX_EAX_EDX "\tmov [ecx + eax], edx\n"
#define	MOV_EDX_EAX		"\tmov edx, eax\n"
#define	MOV_EAX_ECX		"\tmov eax, ecx\n"
#define	MOV_EAX_EBX		"\tmov eax, ebx\n"
#define	MOV_EBX_EAX		"\tmov ebx, eax\n"
#define	MOV_EBX_EDX		"\tmov ebx, edx\n"
#define	MOV_ECX_EAX		"\tmov ecx, eax\n"
#define ADD_ECX_EAX		"\tadd ecx, eax\n"
#define	MOV_EAX_EDX		"\tmov eax, edx\n"
#define ADD_EAX_EDX		"\tadd eax, edx\n"
#define PTR				"\ptr "
#define PREFERRED_ECX	"[ecx]\n"
#define PREFERRED_ECX_C	"[ecx]"
#define PREFERRED_EAX_C	"[eax]"
// exceptions

#define CMP_EBX_0	"\tcmp ebx, 0\n"\
					"\tje errordivzero\n"
#define CMP_ECX_0	"\tcmp ecx, 0\n"\
					"\tje errordivzero\n"
#define CMP_EDX_0	"\tcmp edx, 0\n"\
					"\tje errordivzero\n"
///////////////////////////////////////
#define ENDMAIN "end main"
#pragma endregion
#pragma region typeDATA
#define BYTE	"BYTE"
#define DWORD	"DWORD"
#define REAL4	"REAL4"
#define DEFREAL "0.0"
#define DUP0	" DUP(0)"
#pragma endregion
///				PROTO C				////
namespace GTA 
{
	void GeneratorASM(LT::LexTable*, IT::IdTable*);
	void FindAll(std::vector<int>&, std::vector<int>&, std::vector<int>&);
	void PrintDataTime();
	void PrintLiterals(LT::LexTable*, IT::IdTable*);
	void PrintProtos(LT::LexTable*, IT::IdTable*, std::vector<int>&);
	void PrintFunctions(LT::LexTable*, IT::IdTable*, std::vector<int>&);
	void PrintVariables(LT::LexTable*, IT::IdTable*);
	int ProcessingDeclaration(LT::LexTable*, IT::IdTable*, int, bool,bool,bool,bool);
	void PrintChain(LT::LexTable*, IT::IdTable*, int);
	void PrintConst(LT::LexTable*, IT::IdTable*);
	int FillArray(LT::LexTable*, IT::IdTable*, int);
	int ArrayFillIndex(LT::LexTable*, IT::IdTable*, int);
	int PrintCycles(LT::LexTable*, IT::IdTable*, int);
	int PrintWriteFunctions(LT::LexTable*, IT::IdTable*, int);
	int PrintСondition(LT::LexTable*, IT::IdTable*, int, bool, bool);
	int PrintCallFunctions(LT::LexTable*, IT::IdTable*, int);
	int PrintReturn(LT::LexTable*, IT::IdTable*, int);
}

