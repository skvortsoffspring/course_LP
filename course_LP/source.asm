;--22-12-2020 --вторник-- 01:49:19 --
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
sort                    PROTO : DWORD, : DWORD
maxx                    PROTO : DWORD, : DWORD
;--------------- CONST --------------;
.CONST
str_end				EQU		0
endl				dword	0Ah, 0

;-------------USER CONST ------------;
$size_main              EQU       1033

.DATA
;------------- SYSTEM VARIABLE -------------;
output					BYTE	 255 DUP(0)
seed					DWORD	 0
;--------------- SYSTEM ERROR ---------------;
erroverflow 			BYTE      "Error Integer overflow",str_end
errdivbyzero			BYTE      "Integer division by zero",str_end
errrandomefunc			BYTE      "Error parametres second less first or equals",str_end
erroverflowbyte  		BYTE      "Error byte overflow",str_end
errorid1				DWORD	  1
errorid2				DWORD	  2
errorid3				DWORD	  3
errorid4				DWORD	  4
;------------- LITERALS -------------;
literal_4               DWORD     0
literal_7               DWORD     1
literal_8               DWORD     8
literal_16              BYTE      "hello",str_end
literal_21              DWORD     1033
literal_23              DWORD     10
literal_24              DWORD     81
literal_25              DWORD     1101
literal_26              DWORD     68
literal_27              DWORD     98
literal_28              DWORD     11
literal_29              DWORD     12
literal_30              DWORD     45
literal_31              DWORD     40
literal_32              DWORD     18
literal_33              DWORD     6
literal_34              DWORD     9
literal_35              DWORD     42
literal_36              DWORD     72
literal_37              DWORD     4
literal_38              DWORD     15
literal_40              BYTE      "max  element = ",str_end
;------------- VARIABLES ------------;
i_sort                  DWORD     0
j_sort                  DWORD     0
temp_sort               DWORD     0
j_maxx                  DWORD     0
arrmax_maxx             DWORD     0
hello_main              BYTE      255 DUP(0)
i_main                  DWORD     0
j_main                  DWORD     0
temp_main               DWORD     0
h_main                  DWORD     $size_main DUP(0)
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
enderror:	push errorid
	call ExitProcess
ret
@error endp

randome  proc,
@min : DWORD,
@max : DWORD
mov eax, @min
cmp eax, @max
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
mov		ebx, [@max]
sub		ebx, [@min]
inc		ebx
div		ebx
mov		eax, edx
add		eax, [@min]
jmp OK
errorparametres:
invoke @error, errorid3
OK:
ret
randome endp
sort PROC ,x_sort : DWORD, size_sort : DWORD
;(2)i = 0 ; 
	mov eax, literal_4
	mov i_sort	, eax

;(3)j = 0 ; 
	mov eax, literal_4
	mov j_sort	, eax

;(4)temp ; 
;(4)
@f0:
;(5)j = 0 ; 
	mov eax, literal_4
	mov j_sort	, eax

;(6)
@f1:
;(7)z ( x [ j ] > x [ j 1 + ] @ { temp = x [ j 1 + ] ; 
;(8)x [ j ] > x [ j 1 + ] @ { temp = x [ j 1 + ] ; 
	mov ecx, x_sort
	push ecx
;(8)x [ j ] > x [ j 1 + ] @ { temp = x [ j 1 + ] ; 
	mov eax, j_sort
	pop ecx
	imul eax, type x_sort
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
	push eax

;(8)x [ j 1 + ] @ { temp = x [ j 1 + ] ; 
	mov ecx, x_sort
	push ecx
;(8)x [ j 1 + ] @ { temp = x [ j 1 + ] ; 
	mov eax, j_sort
	mov ebx, literal_7
	add eax, ebx
	pop ecx
	imul eax, type x_sort
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
	pop ebx
	cmp eax, ebx	
jge @f2	; >
;(9)temp = x [ j 1 + ] ; 
	mov ecx, x_sort
	push ecx
;(10)x [ j 1 + ] ; 
	mov eax, j_sort
	mov ebx, literal_7
	add eax, ebx
	pop ecx
	imul eax, type x_sort
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
	mov temp_sort	, eax

;(10)
;(11)x [ j ] ; 
	mov ecx, x_sort
	push ecx
;(11)x [ j ] ; 
	mov eax, j_sort
	pop ecx
	imul eax, type x_sort
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx

	push eax
	mov ecx, x_sort
	mov ebx, type x_sort
	push ebx
;(11)[ j 1 + ] = x [ j ] ; 
	mov eax, j_sort
	mov ebx, literal_7
	add eax, ebx
	pop ebx
	imul ebx
	add ecx, eax
	pop eax
	mov [ecx]	, eax

;(11)
;(12)temp ; 
	mov eax, temp_sort
	mov temp_sort	, eax

	push eax
	mov ecx, x_sort
	mov ebx, type x_sort
	push ebx
;(12)[ j ] = temp ; 
	mov eax, j_sort
	pop ebx
	imul ebx
	add ecx, eax
	pop eax
	mov [ecx]	, eax

@f2:
;(13)j ++ ; 
	inc j_sort
	mov eax, j_sort
	mov ebx, literal_8
	cmp eax, ebx	
jle @f1	;	<
;(15)
	inc i_sort
	mov eax, i_sort
	mov ebx, literal_8
	cmp eax, ebx	
jle @f0	;	<
;(17)
;(18)0 ; 
	mov eax, literal_4


	jmp OK
	errordivzero:
	invoke @error, errorid1
	overflow:
	invoke @error, errorid2
	overflowbyte:
	invoke @error, errorid4
OK:
ret
sort ENDP

maxx PROC ,x_maxx : DWORD, size_maxx : DWORD
;(21)j ; 
;(22)arrmax = 0 ; 
	mov eax, literal_4
	mov arrmax_maxx	, eax

;(22)
@f3:
;(23)z ( x [ j ] > arrmax ) { arrmax = x [ j ] ; 
;(24)x [ j ] > arrmax ) { arrmax = x [ j ] ; 
	mov ecx, x_maxx
	push ecx
;(24)x [ j ] > arrmax ) { arrmax = x [ j ] ; 
	mov eax, j_maxx
	pop ecx
	imul eax, type x_maxx
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
	mov ebx, arrmax_maxx
	cmp eax, ebx	
jge @f4	; >
;(25)arrmax = x [ j ] ; 
	mov ecx, x_maxx
	push ecx
;(26)x [ j ] ; 
	mov eax, j_maxx
	pop ecx
	imul eax, type x_maxx
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
	mov arrmax_maxx	, eax

@f4:
;(27)j ++ ; 
	inc j_maxx
	mov eax, j_maxx
	mov ebx, literal_8
	cmp eax, ebx	
jle @f3	;	<
;(29)
;(30)arrmax ; 
	mov eax, arrmax_maxx
	mov arrmax_maxx	, eax


	jmp OK
	errordivzero:
	invoke @error, errorid1
	overflow:
	invoke @error, errorid2
	overflowbyte:
	invoke @error, errorid4
OK:
ret
maxx ENDP

main PROC 
;(34)hello = hello ; 
	push offset literal_16
	push lengthof literal_16
	cld
	pop ecx
	pop esi
	lea edi, hello_main
	rep movsb

;(35)i = 0 ; 
	mov eax, literal_4
	mov i_main	, eax

;(36)j = 0 ; 
	mov eax, literal_4
	mov j_main	, eax

;(37)temp ; 
;(39)h [ $size ] = { 10 , 81 , 1101 , 68 , 98 , 11 , 12 , 12 , 45 , 45 } ; 
;(39)h [ $size ] = { 10 , 81 , 1101 , 68 , 98 , 11 , 12 , 12 , 45 , 45 } ; 
	xor eax, eax
	mov ebx, type h_main
	mov ecx, offset h_main

;[0]=10
	mov edx, literal_23
	mov [ecx + eax], edx

;[1]=81
	add eax, ebx
	mov edx, literal_24
	mov [ecx + eax], edx

;[2]=1101
	add eax, ebx
	mov edx, literal_25
	mov [ecx + eax], edx

;[3]=68
	add eax, ebx
	mov edx, literal_26
	mov [ecx + eax], edx

;[4]=98
	add eax, ebx
	mov edx, literal_27
	mov [ecx + eax], edx

;[5]=11
	add eax, ebx
	mov edx, literal_28
	mov [ecx + eax], edx

;[6]=12
	add eax, ebx
	mov edx, literal_29
	mov [ecx + eax], edx

;[7]=12
	add eax, ebx
	mov edx, literal_29
	mov [ecx + eax], edx

;[8]=45
	add eax, ebx
	mov edx, literal_30
	mov [ecx + eax], edx

;[9]=45
	add eax, ebx
	mov edx, literal_30
	mov [ecx + eax], edx

;(41)40 [ $size ] = { 40 , 1 , 18 , 6 , 9 , 11 , 42 , 72 , 4 , 15 } ; 
	mov eax, $size_main
;(41)
;(42)$size ) ; 
	mov eax, $size_main
	push eax
	mov ecx, offset h_main
	push ecx
call sort
	mov i_main	, eax

;(42)
;(43)$size ) ; 
	mov eax, $size_main
	push eax
	mov ecx, offset h_main
	push ecx
call maxx
	mov temp_main	, eax

;(43)
;(44)( max  element =  , temp ) ; 
invoke write, ADDR literal_40
;(44), temp ) ; 
invoke int_to_char, ADDR output,temp_main
invoke write, ADDR output
;(44)
	mov eax, literal_4
	mov i_main	, eax

;(45)
@f5:
;(46) ( h [ i ] ) ; 
;(47)( h [ i ] ) ; 
;(47)h [ i ] ) ; 
	mov ecx, offset h_main
	push ecx
;(47)h [ i ] ) ; 
	mov eax, i_main
	pop ecx
	imul eax, type h_main
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
invoke int_to_char, ADDR output	, eax
invoke writeline, ADDR output

;(47)
	inc i_main
	mov eax, i_main
	mov ebx, literal_34
	cmp eax, ebx	
jle @f5	;	<
;(49)
invoke write, ADDR endl
;(50)
;(51)0 ; 
	mov eax, literal_4


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