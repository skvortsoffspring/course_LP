;--16-12-2020 --среда-- 22:02:49 --
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
erroverflow 			BYTE      "Error Integer overflow",str_end
errdivbyzero			BYTE      "Integer division by zero",str_end
errrandomefunc			BYTE      "Error parametres second less first or equals",str_end
erroverflowbyte		BYTE      "Error byte overflow",str_end
errorid1				DWORD	  1
errorid2				DWORD	  2
errorid3				DWORD	  3
errorid4				DWORD	  4
;------------- LITERALS -------------;
literal_2               DWORD     20
literal_3               DWORD     0
literal_4               DWORD     5
literal_5               DWORD     2
literal_6               DWORD     13
literal_7               DWORD     4
literal_8               DWORD     15
literal_9               DWORD     6
literal_10              DWORD     7
literal_11              DWORD     8
literal_12              DWORD     91
literal_13              DWORD     1111
literal_16              DWORD     1
literal_17              DWORD     10
;------------- VARIABLES ------------;
x_main                  DWORD     20 DUP(0)
y_main                  DWORD     0
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
cmp errorid4,eax
je testerror4
invoke write, addr erroverflowbyte
jmp enderror
testerror4:
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
;(2)
xor eax, eax
mov ebx, type x_main
mov ecx, offset x_main

;[0]=0
mov edx, literal_3
mov [ecx + eax], edx

;[1]=5
add eax, ebx
mov edx, literal_4
mov [ecx + eax], edx

;[2]=2
add eax, ebx
mov edx, literal_5
mov [ecx + eax], edx

;[3]=13
add eax, ebx
mov edx, literal_6
mov [ecx + eax], edx

;[4]=4
add eax, ebx
mov edx, literal_7
mov [ecx + eax], edx

;[5]=15
add eax, ebx
mov edx, literal_8
mov [ecx + eax], edx

;[6]=6
add eax, ebx
mov edx, literal_9
mov [ecx + eax], edx

;[7]=7
add eax, ebx
mov edx, literal_10
mov [ecx + eax], edx

;[8]=8
add eax, ebx
mov edx, literal_11
mov [ecx + eax], edx

;[9]=91
add eax, ebx
mov edx, literal_12
mov [ecx + eax], edx

;[10]=1111
add eax, ebx
mov edx, literal_13
mov [ecx + eax], edx

;(3)
mov eax, literal_3
mov y_main, eax

@f0:
;(5)
;(5)
mov ecx, offset x_main
push ecx
;(5)
mov eax, y_main
pop ecx
imul eax, type x_main
add ecx, eax
mov edx, [ecx]
mov eax, edx
invoke int_to_char, ADDR output, eax
invoke writeline, ADDR output

;(5)
mov eax, y_main
mov ebx, literal_16
add eax, ebx
jc overflow
mov y_main, eax

mov eax, y_main
mov ebx, literal_17
cmp eax, ebx	
jbe @f0	;	<=

jmp OK
errordivzero:
invoke @error, errorid1
overflow:
invoke @error, errorid2
overflowbyte:
invoke @error, errorid4
OK:
push 0
call ExitProcess
main ENDP

write proc uses eax ebx ecx edi esi,
pstr	:	dword
push - 11
call GetStdHandle
mov esi, pstr
mov edi, -1
count:
inc edi
cmp byte ptr[esi + edi], 0
jne count
push 0
push 0
push edi
push pstr
push eax
call WriteConsoleA
ret
write ENDP

writeline proc uses eax ebx ecx edi esi,
pstr	:	dword
push - 11
call GetStdHandle
mov esi, pstr
mov edi, -1
count :
inc edi
cmp byte ptr[esi + edi], 0
jne count
push 0
push 0
push edi
push pstr
push eax
call WriteConsoleA
push - 11
call GetStdHandle
push 0
push 0
push 1
push offset endl
push eax
call WriteConsoleA
ret
writeline ENDP

int_to_char proc uses eax ebx ecx edi esi,
pstr : dword,
intfield : dword
mov edi, pstr
mov esi, 0
mov eax, intfield
cdq
mov ebx, 10
idiv ebx
test eax, 80000000h
jz plus
neg eax
neg edx
mov cl, '-'
mov[edi], cl
inc edi
plus: 
push dx
inc esi
test eax, eax
jz fin
cdq
idiv ebx
jmp plus
fin: mov ecx, esi
writ: 
pop bx
add bl, '0'
mov[edi], bl
inc edi
xor ebx, ebx
mov[edi], ebx
loop writ
ret
int_to_char	ENDP

end main