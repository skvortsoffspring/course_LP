#pragma once
#include <vector>
#define FILE			"source.asm"								// file for generation
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
						"erroverflowbyte		BYTE      \"Error byte overflow\",str_end\n"   \
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

#define EXIT	"push 0\n"																	\
				"call ExitProcess\n"

#define ERROR_ASM "jmp OK\n"																\
				"errordivzero:\n"															\
				"invoke @error, errorid1\n"													\
				"overflow:\n"																\
				"invoke @error, errorid2\n"													\
				"overflowbyte:\n"																\
				"invoke @error, errorid4\n"													\
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
#define LITERALS			"literal"
#define ID_IN_IDTABLE	"idtable->table[lextable->table[poslenta].idxTI].id"
#define TYPE_IN_IDTABLE	"idtable->table[lextable->table[poslenta].idxTI].idtype"
#pragma endregion


#pragma region commandsASM
// standarts
#define PROC		"PROC"
#define ENDP		"ENDP\n"
#define START		"START :\n"
#define RET			"ret\n"
#define EQU			"EQU"
// errors macroses
#define OVERFLOW_ASM		"jc overflow\n"

#define OVERFLOW_ASM_BYTE	"cmp eax, 0100h\n"\
							"jge overflowbyte\n"

// registers
#define EAX			", eax"
#define AL			", al"
#define EDX			"edx"
#define DL			", dl"
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

#define DIV_EAX_EDX "idiv edx\n"		// делелить
#define DIV_EAX_ECX "idiv ecx\n"		// делелить
#define DIV_EAX_EBX "idiv ebx\n"		// делелить
#define DIV_EBX_ECX "idiv ebx, ecx\n"	// делелить
#define DIV_ECX_EDX "idiv ecx, edx\n"	// делелить

#define MUL_EAX_EBX "imul ebx\n"		// умножить
#define MUL_EBX_ECX "imul ebx, ecx\n"	// умножить
#define MUL_ECX_EDX "imul ecx, edx\n"	// умножить

//operations byte
#define ADD_AL_BL	"add al, bl\n"		// сложить
#define SUB_AL_BL	"sub al, bl\n"		// вычесть
#define DIV_AL_BL	"idiv al, bl\n"		// делить
#define MUL_AL_BL	"imul al, bl\n"		// умножить

// logic
#define XOR_EAX			"xor eax, eax\n"
#define XOR_EBX			"xor ebx, ebx\n"
#define XOR_ECX			"xor ecx, ecx\n"
#define XOR_EDX			"xor edx, edx\n"
// cmp
#define CMP_EAX_EBX		"cmp eax, ebx	\n"
#define CMP_EBX_ECX		"cmp ebx, ecx	\n"
#define CMP_ECX_EDX		"cmp ecx, edx	\n"
#define CMP_EAX_EDX		"cmp eax, edx	\n"
// array 
#define MOV_ECX_ADDR	"mov ecx, offset "
#define MOV_EDX_ADDR	"mov edx, offset "

#define MOV_EBX_TYPE	"mov ebx, type "
#define IMUL_EAX_TYPE	"imul eax, type "
#define MOV_ECX_EAX_EDX "mov [ecx + eax], edx\n"
#define	MOV_EDX_EAX		"mov edx, eax\n"
#define	MOV_EAX_ECX		"mov eax, ecx\n"
#define	MOV_EAX_EBX		"mov eax, ebx\n"
#define	MOV_EBX_EAX		"mov ebx, eax\n"
#define	MOV_EBX_EDX		"mov ebx, edx\n"
#define	MOV_ECX_EAX		"mov ecx, eax\n"
#define ADD_ECX_EAX		"add ecx, eax\n"
#define	MOV_EAX_EDX		"mov eax, edx\n"
#define ADD_EAX_EDX		"add eax, edx\n"
#define PTR				"ptr "
#define PREFERRED_ECX	"[ecx]\n"
#define PREFERRED_ECX_C	"[ecx]"
// exceptions

#define CMP_EBX_0	"cmp ebx, 0\n"\
					"je errordivzero\n"
#define CMP_ECX_0	"cmp ecx, 0\n"\
					"je errordivzero\n"
#define CMP_EDX_0	"cmp edx, 0\n"\
					"je errordivzero\n"
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
"count :\n"
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
"mov eax, errorid\n"
"cmp errorid1, eax\n"
"je testerror1\n"
"invoke write, addr erroverflow\n"
"jmp enderror\n"
"testerror1:\n"
"cmp errorid2,eax\n"
"je testerror2\n"
"invoke write, addr errdivbyzero\n"
"jmp enderror\n"
"testerror2:\n"
"cmp errorid3,eax\n"
"je testerror3\n"
"invoke write, addr errrandomefunc\n"
"jmp enderror\n"
"testerror3:\n"
"cmp errorid4,eax\n"
"je testerror4\n"
"invoke write, addr erroverflowbyte\n"
"jmp enderror\n"
"testerror4:\n"
"enderror:"
"push errorid\n"
"call ExitProcess\n"
"ret\n"
"@error endp\n\n";

constexpr auto RANDOME_FUNC =
"GetRandomNumber  proc uses eax ebx ecx edi esi,\n"
"min : DWORD,\n" 
"max : DWORD\n"
"mov eax, min\n"
"cmp eax, max\n"
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
"mov		ebx, [max]\n"
"sub		ebx, [min]\n"
"inc		ebx\n"
"div		ebx\n"
"mov		eax, edx\n"
"add		eax, [min]\n"
"mov[seed], eax\n"
"pop		edx\n"
"pop ecx\n"
"pop ebx\n"
"jmp OK\n"																\
"errorparametres:\n"															\
"invoke @error, errorid3\n"													\
"OK:\n"
"ret\n"
"GetRandomNumber endp\n";

namespace GTA 
{
	void GeneratorASM(LT::LexTable*, IT::IdTable*);
	void FindAll(std::vector<int>&, std::vector<int>&, std::vector<int>&);
	void PrintDataTime();
	void PrintLiterals(LT::LexTable*, IT::IdTable*);
	void PrintProtos(LT::LexTable*, IT::IdTable*, std::vector<int>&);
	void PrintFunctions(LT::LexTable*, IT::IdTable*, std::vector<int>&);
	void PrintVariables(LT::LexTable*, IT::IdTable*);
	int ProcessingDeclaration(LT::LexTable*, IT::IdTable*, int, bool,bool,bool);
	void PrintChain(LT::LexTable*, IT::IdTable*, int);
	void PrintConst(LT::LexTable*, IT::IdTable*);
	int FillArray(LT::LexTable*, IT::IdTable*, int);
	int ArrayFillIndex(LT::LexTable*, IT::IdTable*, int);
	int PrintCycles(LT::LexTable*, IT::IdTable*, int);
	int PrintWriteFunctions(LT::LexTable*, IT::IdTable*, int);
	int PrintСondition(LT::LexTable*, IT::IdTable*, int, bool, bool);


}

