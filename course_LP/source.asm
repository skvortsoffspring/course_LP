;--16-12-2020 --среда-- 10:54:45 --
;-------------ASM-------------;
.586p
.model flat, stdcall
.stack 4096

includelib kernel32.lib
includelib libucrt.lib

;---------- STANDART PROTO ----------;
WriteConsoleA			PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD
MessageBoxA  			PROTO : DWORD, : DWORD, : DWORD, : DWORD
write					PROTO : DWORD
writeline				PROTO : DWORD
int_to_char  			PROTO : DWORD, : DWORD
GetStdHandle			PROTO : DWORD
SetConsoleOutputCP		PROTO : DWORD
SetConsoleCP			PROTO : DWORD
GetRandomNumber  		PROTO : DWORD, : DWORD
ExitProcess  			PROTO : DWORD

;------------ PPOTO USER ------------;
;--------------- CONST --------------;
.CONST
str_end				EQU		0
endl				dword	0Ah, 0

;-------------USER CONST ------------;

.DATA
;------------- SYSTEM VARIABLE -------------;
output					BYTE	 255 DUP(0)
seed					DWORD	 0
;--------------- SYSTEM ERROR ---------------;
erroverflow 			BYTE      "Integer overflow",str_end
errdivbyzero			BYTE      "Integer division by zero",str_end
errrandomefunc			BYTE      "Error parametres second less first or equals",str_end
errorid1				DWORD	  1
errorid2				DWORD	  2
errorid3				DWORD	  3
;------------- LITERALS -------------;
literal_2               DWORD     10
literal_3               DWORD     1
literal_4               DWORD     3
literal_5               DWORD     0
literal_6               DWORD     2
literal_8               DWORD     9
;------------- VARIABLES ------------;
x_main                  DWORD     10 DUP(0)
;--------------- CODE ---------------;
.CODE
@error proc, errorid : dword
mov eax, errorid
cmp errorid1, eax
je testerror1
invoke write, addr erroverflow
jmp enderror
testerror1:
cmp errorid2,eax
je testerror2
invoke write, addr errdivbyzero
jmp enderror
testerror2:
cmp errorid3,eax
je testerror3
invoke write, addr errrandomefunc
jmp enderror
testerror3:
enderror:push errorid
call ExitProcess
ret
@error endp

GetRandomNumber  proc uses eax ebx ecx edi esi,
min : DWORD,
max : DWORD
mov eax, min
cmp eax, max
jae errorparametres
mov			eax, [seed]
or eax, eax
jnz		@2
@1:
RDTSC
or eax, eax
jz		@1
@2:
xor edx, edx
mov		ebx, 127773
div		ebx
push		eax
mov		eax, 16807
mul		edx
pop		edx
push		eax
mov		eax, 2836
mul		edx
pop		edx
sub		edx, eax
mov		eax, edx
mov[seed], edx
xor edx, edx
mov		ebx, [max]
sub		ebx, [min]
inc		ebx
div		ebx
mov		eax, edx
add		eax, [min]
mov[seed], eax
pop		edx
pop ecx
pop ebx
jmp OK
errorparametres:
invoke @error, errorid3
OK:
ret
GetRandomNumber endp
main PROC 
