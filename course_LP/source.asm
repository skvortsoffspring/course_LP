;--10-12-2020 --четверг-- 23:51:49 --
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
literal_4               DWORD     18
literal_6               DWORD     10
literal_7               DWORD     1
literal_8               DWORD     9
literal_9               DWORD     20
literal_11              DWORD     2
literal_12              DWORD     0
literal_14              REAL4     0.14
literal_16              REAL4     3.14
literal_18              REAL4     1.61
literal_20              DWORD     255
literal_23              DWORD     29
literal_24              DWORD     40
literal_26              DWORD     12
literal_33              BYTE      "ss",str_end
literal_41              BYTE      "Результат  x+*!~y = ",str_end
literal_44              BYTE      "asdasdss",str_end
literal_50              BYTE      "Результат = ",str_end
;------------- VARIABLES ------------
gerty_sorting           BYTE      0
index_sorting           DWORD     0
k_sorting               DWORD     0
m_sorting               REAL4     0.0
pi_sorting              REAL4     0.0
ideal_sorting           REAL4     0.0
summ_sorting            DWORD     0
jokes_sorting           DWORD     0
glob_a_sorting          DWORD     0
v_sorting               BYTE      0
b_sorting               BYTE      0
c_sorting               BYTE      0
e_sorting               BYTE      0
d_sorting               BYTE      0
msys_sorting            DB        255 DUP(0)
index_sort              BYTE      0
indexsec_sort           BYTE      0
result_sort             DWORD     0
msys_sort               DB        255 DUP(0)
temp_sort               DWORD     0
mss_sort                DB        255 DUP(0)
size_main               BYTE      0
index_main              BYTE      0
result_main             DB        255 DUP(0)
avg_main                DWORD     0
temp_main               DWORD     0
;--------------- CODE ---------------
.CODE
main PROC 
START :
push 0
push 0
call sorting
;size = 20 ; 
mov eax, literal_9
mov size_main, al
;array [ size ] ; 
mov bl, size_main
;index ; 
;result = Результат =  ; 
;array [ index ] = avg ( size + 1 , size , array ) ; 
mov bl, index_main
mov eax, avg_main
mov al, size_main
add eax, ebx
mov ebx, literal_7
mov bl, size_main
;index < size ) ; 
mov bl, size_main
;avg = temp ( array , size ) ; 
mov eax, temp_main
mov bl, size_main
mov avg_main, eax
;
mov eax, literal_12
mov index_main, al
;array [ index ] > avg ) { t temp ; 
mov bl, index_main
mov eax, avg_main
mov ebx, temp_main
;
mov bl, index_main
;
mov bl, index_main
mov al, index_main
mov eax, literal_7
;
mov bl, index_main
mov eax, literal_7
mov ebx, temp_main
;index n ; 

ret
main ENDP

sorting PROC ,array_sorting : REAL4, size_sorting : DWORD
;gerty = 18 ; 
mov eax, literal_4
mov gerty_sorting, al
;index = 10 1 + 9 * 20 / gerty + ; 
mov eax, literal_6
mov ebx, literal_7
add eax, ebx
mov ebx, literal_8
mul ebx
mov ebx, literal_9
div ebx
mov bl, gerty_sorting
add eax, ebx
mov index_sorting, eax
;k = 10 ; 
mov eax, literal_6
mov k_sorting, eax
;
mov eax, index_sorting
mov ebx, literal_6
sub eax, ebx
mov index_sorting, eax
;index = 1 ; 
mov eax, literal_7
mov index_sorting, eax
;index < size ) ; 
;size > 2 ) { size = 0 ; 
mov eax, literal_11
mov ebx, literal_12
mov size_sorting, eax

ret
sorting ENDP

sort PROC ,array_sort : DWORD, size_sort : DWORD
;index ; 
;indexsec ; 
;result ; 
;msys = Результат  x+*!~y =  ; 
;indexsec = index ; 
mov bl, index_sort
;temp ; 
;
mov bl, index_sort
;
mov bl, index_sort
mov al, index_sort
;
mov bl, index_sort
mov eax, temp_sort
mov array_sort, eax
;indexsec < size ) ; 
;
;index < size ) ; 
;mss = asdasdss ; 
;result ; 

ret
sort ENDP

end main