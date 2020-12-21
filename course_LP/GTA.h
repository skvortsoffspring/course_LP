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
						"erroverflow 			BYTE      \"Error Integer overflow\",str_end\n"			\
						"errdivbyzero			BYTE      \"Integer division by zero\",str_end\n"   \
						"errrandomefunc			BYTE      \"Error parametres second less first or equals\",str_end\n"   \
						"erroverflowbyte  		BYTE      \"Error byte overflow\",str_end\n"   \
						"errorid1				DWORD	  1\n"									\
						"errorid2				DWORD	  2\n"									\
						"errorid3				DWORD	  3\n"									\
						"errorid4				DWORD	  4\n"									\

#define DATA	".DATA\n"												\
				";------------- SYSTEM VARIABLE -------------;\n"		\
				"output					BYTE	 255 DUP(0)\n"	\
				"seed					DWORD	 0\n"

#define CODE	".CODE\n"

#define LIBRARY "includelib kernel32.lib\n"							\
				"includelib libucrt.lib\n\n"										

#define HEAD_ASM ";-------------ASM-------------;\n"					\
				  ".586p\n"											\
				  ".model flat, stdcall\n"							\
				  ".stack 4096\n"									\
				  "\n"
#define PROTO   ";---------- STANDART PROTO ----------;\n"\
				"WriteConsoleA			PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD\n"\
				"MessageBoxA  			PROTO : DWORD, : DWORD, : DWORD, : DWORD\n"			\
				"write					PROTO : DWORD\n"									\
				"writeline				PROTO : DWORD\n"									\
				"int_to_char  			PROTO : DWORD, : DWORD\n"							\
				"GetStdHandle			PROTO : DWORD\n"									\
				"SetConsoleOutputCP		PROTO : DWORD\n"									\
				"SetConsoleCP			PROTO : DWORD\n"									\
				"GetRandomNumber  		PROTO : DWORD, : DWORD\n"							\
				"ExitProcess  			PROTO : DWORD\n\n"

#define EXIT	"\tpush 0\n"																	\
				"\tcall ExitProcess\n"

#define ERROR_ASM "\tjmp OK\n"																\
				"\terrordivzero:\n"															\
				"\tinvoke @error, errorid1\n"													\
				"\toverflow:\n"																\
				"\tinvoke @error, errorid2\n"													\
				"\toverflowbyte:\n"																\
				"\tinvoke @error, errorid4\n"													\
				"OK:\n"
#define DEVISION_OVERFLOW	";--- division overflow ---;\n"
	
//invoke int_to_char, ADDR output, [ecx]
//invoke write, ADDR output
#pragma region shortTEXT
#define SPP				", : "		// separator parametres prototypes
#define SFP				" : "		// separator functions parametres
#define COLON			":"
#define UNDERLINE		"_"
#define STR_END			"str_end\n"
#define LITERALS		"literal"
#define ID_IN_IDTABLE	"idtable->table[lextable->table[poslenta].idxTI].id"
#define TYPE_IN_IDTABLE	"idtable->table[lextable->table[poslenta].idxTI].idtype"
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
// errors macroses
#define OVERFLOW_ASM		"\tjc overflow\n"

#define OVERFLOW_ASM_BYTE	"\tcmp eax, 0100h\n"\
							"\tjge overflowbyte\n"
// increment decrement
#define INCREMENT	"\tinc "
#define DECREMENT	"\tdec "
// registers		 \t
#define EAX			"\t, eax"
#define AL			"\t, al"
#define EDX			"\tedx"
#define DL			"\t, dl"
// stack			 \t
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
///////////////////////// FUNCTIONS ASM ///////////////////////////////////
constexpr auto WRITE = 
"write proc uses eax ebx ecx edi esi,\n"								
"pstr	:	dword\n"														
"push - 11\n"																
"call GetStdHandle\n"														
"mov esi, pstr\n"															
"mov edi, -1\n"															
"count:\n"																
"inc edi\n"																
"cmp byte ptr[esi + edi], 0\n"											
"jne count\n"																
"push 0\n"																
"push 0\n"																
"push edi\n"																
"push pstr\n"																
"push eax\n"																
"call WriteConsoleA\n"													
"ret\n"																	
"write ENDP\n";

constexpr auto WRITELINE =
"writeline proc uses eax ebx ecx edi esi,\n"
"pstr	:	dword\n"
"push - 11\n"
"call GetStdHandle\n"
"mov esi, pstr\n"
"mov edi, -1\n"
"count:\n"
"inc edi\n"
"cmp byte ptr[esi + edi], 0\n"
"jne count\n"
"push 0\n"
"push 0\n"
"push edi\n"
"push pstr\n"
"push eax\n"
"call WriteConsoleA\n"
"push - 11\n"
"call GetStdHandle\n"
"push 0\n"
"push 0\n"
"push 1\n"
"push offset endl\n"
"push eax\n"
"call WriteConsoleA\n"
"ret\n"
"writeline ENDP\n";

constexpr auto INT_TO_CHAR =	
"int_to_char proc uses eax ebx ecx edi esi,\n"
"pstr : dword,\n"																													
"intfield : dword\n"														
"mov edi, pstr\n"														
"mov esi, 0\n"															
"mov eax, intfield\n"													
"cdq\n"																	
"mov ebx, 10\n"															
"idiv ebx\n"																
"test eax, 80000000h\n"													
"jz plus\n"																
"neg eax\n"																
"neg edx\n"																
"mov cl, '-'\n"															
"mov[edi], cl\n"															
"inc edi\n"																
"plus: \n"															
"push dx\n"																
"inc esi\n"																
"test eax, eax\n"														
"jz fin\n"																
"cdq\n"																	
"idiv ebx\n"																
"jmp plus\n"																
"fin: mov ecx, esi\n"													
"writ: \n"																
"pop bx\n"																
"add bl, '0'\n"															
"mov[edi], bl\n"															
"inc edi\n"
"xor ebx, ebx\n"	
"mov[edi], ebx\n"	
"loop writ\n"															
"ret\n"																	
"int_to_char	ENDP\n";

constexpr auto FUNCTION_ERROR = 
"@error proc, errorid : dword\n"
"\tmov eax, errorid\n"
"\tcmp errorid1, eax\n"
"\tje testerror1\n"
"\tinvoke write, addr erroverflow\n"
"\tjmp enderror\n"
"\ttesterror1:\n"
"\tcmp errorid2,eax\n"
"\tje testerror2\n"
"invoke write, addr errdivbyzero\n"
"\tjmp enderror\n"
"testerror2:\n"
"\tcmp errorid3,eax\n"
"\tje testerror3\n"
"\tinvoke write, addr errrandomefunc\n"
"\tjmp enderror\n"
"testerror3:\n"
"\tcmp errorid4,eax\n"
"je testerror4\n"
"\tinvoke write, addr erroverflowbyte\n"
"\tjmp enderror\n"
"testerror4:\n"
"enderror:"
"\tpush errorid\n"
"\tcall ExitProcess\n"
"ret\n"
"@error endp\n\n";

constexpr auto RANDOME_FUNC =
"randome  proc,\n"
"@min : DWORD,\n" 
"@max : DWORD\n"
"mov eax, @min\n"
"cmp eax, @max\n"
"jae errorparametres\n"
"mov			eax, [seed]\n"
"or eax, eax\n"
"jnz		@2\n"
"@1:\n"
"RDTSC\n"
"or eax, eax\n"
"jz		@1\n"
"@2:\n"
"xor edx, edx\n"
"mov		ebx, 127773\n"
"div		ebx\n"
"push		eax\n"
"mov		eax, 16807\n"
"mul		edx\n"
"pop		edx\n"
"push		eax\n"
"mov		eax, 2836\n"
"mul		edx\n"
"pop		edx\n"
"sub		edx, eax\n"
"mov		eax, edx\n"
"mov[seed], edx\n"
"xor edx, edx\n"
"mov		ebx, [@max]\n"
"sub		ebx, [@min]\n"
"inc		ebx\n"
"div		ebx\n"
"mov		eax, edx\n"
"add		eax, [@min]\n"
"jmp OK\n"																\
"errorparametres:\n"															\
"invoke @error, errorid3\n"													\
"OK:\n"
"ret\n"
"randome endp\n";

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

