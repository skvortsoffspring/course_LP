;--13-12-2020 --воскресенье-- 17:32:26 --
;-------------ASM-------------
.586p
.model flat, stdcall
.stack 4096

includelib kernel32.lib
includelib libucrt.lib

;---------- STANDART PROTO ----------
WriteConsoleA			PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD
MessageBoxA				PROTO : DWORD, : DWORD, : DWORD, : DWORD
write					PROTO : DWORD, : DWORD
writeline				PROTO : DWORD, : DWORD
int_to_char				PROTO : DWORD, : DWORD
GetStdHandle			PROTO : DWORD
SetConsoleOutputCP		PROTO : DWORD
SetConsoleCP			PROTO : DWORD
ExitProcess				PROTO : DWORD

;------------ PPOTO USER ------------
sorting                 PROTO : REAL4, : DWORD
sort                    PROTO : DWORD, : DWORD
;--------------- CONST --------------
.CONST
str_end		EQU		0
endl		dword	0Ah, 0

.DATA
;------------- LITERALS -------------
literal_4               DWORD     0
literal_6               DWORD     234
literal_8               DWORD     9
literal_9               DWORD     8
literal_10              DWORD     7
literal_11              DWORD     6
literal_13              DWORD     16
literal_14              DWORD     2
literal_15              DWORD     12
literal_16              DWORD     0
literal_23              DWORD     1
;------------- VARIABLES ------------
y_sorting               DWORD     247111111
qerty_sorting           BYTE      0
x_sorting               DWORD     10 DUP(0)
index_sorting           DWORD     0
h_sort                  DWORD     0
y_main                  DWORD     0
;--------------- CODE ---------------
.CODE
sorting PROC ,array_sorting : REAL4, size_sorting : DWORD
;(3)y = 10 ; 
Overflow:
mov eax, literal_6
mov ebx, literal_4
idiv ebx
cmp ebx, 0
je error

mov y_sorting, eax
jo Overflow
error:
;(4)qerty = 18 y + ; 
mov eax, literal_6
mov ebx, y_sorting
add eax, ebx
cmp eax, 0ffh
ja Overflow
mov qerty_sorting, al 
;cmp jo
;(5)x [ 10 ] = { 10 , 9 , 8 , 7 , 6 } ; 
;(6)index = 16 16 * 2 * 12 - x [ 0 ] + ; 
mov eax, literal_13
mov ebx, literal_13
imul eax, ebx
mov ebx, literal_14
imul eax, ebx
mov ebx, literal_15
sub eax, ebx
push eax
mov ecx, offset x_sorting
push ecx
mov eax, literal_16
pop ecx
imul eax, type x_sorting
add ecx, eax
mov edx, [ecx]
pop eax
mov ebx, edx
add eax, ebx
mov index_sorting, eax
;(6)
mov eax, literal_16

ret
sorting ENDP

sort PROC ,array_sort : DWORD, size_sort : DWORD
;(11)h = 10 ; 
mov eax, literal_4
mov h_sort, eax
 mov eax, 0
ret
sort ENDP

main PROC 
call sorting
;(15)y = 1 ; 
mov eax, literal_23
mov y_main, eax

ret
main ENDP

end main