;--30-12-2020 --среда-- 14:56:38 --
;-------------ASM-------------;
.586p
.model flat, stdcall
.stack 4096

includelib kernel32.lib
includelib libucrt.lib
includelib user32.lib
includelib ucrt.lib

includelib ../Debug/string.lib
includelib ../Debug/std.lib
includelib ../Debug/error.lib
includelib ../Debug/random.lib
;---------- STANDART PROTO ----------;
int_to_char  			PROTO : DWORD, : DWORD
strcat		   			PROTO : DWORD, : DWORD
strcopy				   	PROTO : DWORD, : DWORD
write					PROTO : DWORD
writeline				PROTO : DWORD
GetStdHandle			PROTO : DWORD
SetConsoleOutputCP		PROTO : DWORD
strlen			 		PROTO : DWORD
SetConsoleCP			PROTO : DWORD
@error					PROTO : DWORD
random			 		PROTO : DWORD, : DWORD
ExitProcess  			PROTO : DWORD

;------------ PPOTO USER ------------;
sort                    PROTO : DWORD, : DWORD
replacestr              PROTO : DWORD, : DWORD
summarr                 PROTO : DWORD
;--------------- CONST --------------;
.CONST
str_end				EQU		0
endl				dword	0Ah, 0

;-------------USER CONST ------------;
$size_main              EQU       10

.DATA
;------------- SYSTEM VARIABLE -------------;
output					BYTE	 255 DUP(0)
;--------------- SYSTEM ERROR ---------------;
errorid1				DWORD	  1
errorid2				DWORD	  2
errorid3				DWORD	  3
errorid4				DWORD	  4
;------------- LITERALS -------------;
literal_4               DWORD     0
literal_7               DWORD     1
literal_8               DWORD     8
literal_14              DWORD     2
literal_20              DWORD     9
literal_26              DWORD     10
literal_28              BYTE      "warlock",str_end
literal_30              BYTE      "murlakatama",str_end
literal_32              BYTE      "hello",str_end
literal_34              BYTE      " ",str_end
literal_36              BYTE      "world",str_end
literal_39              DWORD     81
literal_40              DWORD     111
literal_41              DWORD     68
literal_42              DWORD     98
literal_43              DWORD     11
literal_44              DWORD     12
literal_45              DWORD     45
literal_47              BYTE      "array long: ",str_end
literal_49              BYTE      "x[",str_end
literal_50              BYTE      "]:",str_end
literal_51              BYTE      ",",str_end
literal_54              BYTE      "array sort: ",str_end
literal_60              BYTE      "summ alements array = ",str_end
literal_64              BYTE      "cycle in cicle and if else",str_end
literal_66              BYTE      "#",str_end
literal_68              BYTE      "*",str_end
literal_70              BYTE      "@",str_end
literal_75              BYTE      "fill array byte for random",str_end
literal_77              DWORD     255
literal_79              BYTE      "y[",str_end
literal_83              BYTE      "#function call to replace characters#",str_end
literal_87              DWORD     15
literal_88              DWORD     3
literal_91              BYTE      "10*(2+2)*15/3+strlen(hello) = ",str_end
literal_95              BYTE      "#standart string function#",str_end
literal_97              BYTE      "standart strcat: ",str_end
literal_102             BYTE      "standart strlen : ",str_end
literal_106             BYTE      "standart strcopy: ",str_end
literal_109             BYTE      "string",str_end
literal_110             BYTE      "work",str_end
;------------- VARIABLES ------------;
i_sort                  DWORD     0
j_sort                  DWORD     0
temp_sort               DWORD     0
length_replacestr       BYTE      0
i_replacestr            BYTE      0
i_summarr               BYTE      0
summ_summarr            DWORD     0
temp_summarr            DWORD     0
i_main                  DWORD     0
j_main                  DWORD     0
flag_main               BYTE      0
wlock_main              BYTE      255 DUP(0)
mtama_main              BYTE      255 DUP(0)
hello_main              BYTE      255 DUP(0)
space_main              BYTE      255 DUP(0)
world_main              BYTE      255 DUP(0)
arry_main               BYTE      $size_main DUP(0)
arrx_main               DWORD     10 DUP(0)
;--------------- CODE ---------------;
.CODE
sort PROC ,x_sort : DWORD, size_sort : DWORD


;(2)i = 0 
	mov eax, literal_4
	mov i_sort, eax

;(3)j = 0 
	mov eax, literal_4
	mov j_sort, eax

;(4)temp 
;(4)
@f0:
;(6)j = 0 
	mov eax, literal_4
	mov j_sort, eax

;(6)
@f1:
;(7){ z ( x [ j ] > x [ j 1 + ] ) { temp = x [ j 1 + ] 
;(8)( x [ j ] > x [ j 1 + ] ) { temp = x [ j 1 + ] 
	mov ecx, x_sort
	push ecx
;(8)x [ j ] > x [ j 1 + ] ) { temp = x [ j 1 + ] 
	mov eax, j_sort
	pop ecx
	imul eax, type x_sort
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
	push eax

;(8)> x [ j 1 + ] ) { temp = x [ j 1 + ] 
	mov ecx, x_sort
	push ecx
;(8)x [ j 1 + ] ) { temp = x [ j 1 + ] 
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
jge @f2
;(10)temp = x [ j 1 + ] 
	mov ecx, x_sort
	push ecx
;(10)x [ j 1 + ] 
	mov eax, j_sort
	mov ebx, literal_7
	add eax, ebx
	pop ecx
	imul eax, type x_sort
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
	mov temp_sort, eax

;(11)= x [ j ] 
	mov ecx, x_sort
	push ecx
;(11)x [ j ] 
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
;(11)[ j 1 + ] = x [ j ] 
	mov eax, j_sort
	mov ebx, literal_7
	add eax, ebx
	pop ebx
	imul ebx
	add ecx, eax
	pop eax
	mov [ecx], eax
;(12)= temp 
	mov eax, temp_sort
	mov temp_sort, eax

	push eax
	mov ecx, x_sort
	mov ebx, type x_sort
	push ebx
;(12)[ j ] = temp 
	mov eax, j_sort
	pop ebx
	imul ebx
	add ecx, eax
	pop eax
	mov [ecx], eax
@f2:
;(14)j ++ 
	inc j_sort
	mov eax, j_sort
	mov ebx, literal_8
	cmp eax, ebx	
jle @f1
;(16)i ++ 
	inc i_sort
	mov eax, i_sort
	mov ebx, literal_8
	cmp eax, ebx	
jle @f0
;(17)
	mov eax, x_sort

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

replacestr PROC ,x_replacestr : DWORD, m_replacestr : DWORD


;(22)length 
;(23)i 
;(23)
@f3:
;(25)= m [ i ] 
	mov ecx, m_replacestr
	push ecx
;(25)m [ i ] 
	xor eax, eax
	mov al, i_replacestr
	pop ecx
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx

	push eax
	mov ecx, x_replacestr
	mov ebx, BYTE
	push ebx
;(25)[ i ] = m [ i ] 
	xor eax, eax
	mov al, i_replacestr
	pop ebx
	imul ebx
	add ecx, eax
	pop eax
	mov [ecx], al
;(26)i ++ 
	inc i_replacestr
	xor eax, eax
	mov al, i_replacestr
	mov ebx, literal_14
	cmp eax, ebx	
jne @f3
;(27)
	mov eax, x_replacestr

	jmp OK
	errordivzero:
	invoke @error, errorid1
	overflow:
	invoke @error, errorid2
	overflowbyte:
	invoke @error, errorid4
OK:
ret
replacestr ENDP

summarr PROC ,x_summarr : DWORD


;(32)i 
;(33)summ 
;(34)temp 
;(34)
@f4:
;(36)summ = summ x [ i ] + 
	mov eax, summ_summarr
	push eax
	mov ecx, x_summarr
	push ecx
;(36)x [ i ] + 
	xor eax, eax
	mov al, i_summarr
	pop ecx
	imul eax, type x_summarr
	add ecx, eax
	mov edx, [ecx]
	pop eax
	mov ebx, edx
	add eax, ebx
	mov summ_summarr, eax

;(37)i ++ 
	inc i_summarr
	xor eax, eax
	mov al, i_summarr
	mov ebx, literal_20
	cmp eax, ebx	
jle @f4
;(38)
;(39)r summ 
	mov eax, summ_summarr
	mov summ_summarr, eax


	jmp OK
	errordivzero:
	invoke @error, errorid1
	overflow:
	invoke @error, errorid2
	overflowbyte:
	invoke @error, errorid4
OK:
ret
summarr ENDP

main PROC 

push 1251d
call SetConsoleOutputCP
push 1251d
call SetConsoleCP

;(43)i = 0 
	mov eax, literal_4
	mov i_main, eax

;(44)j = 0 
	mov eax, literal_4
	mov j_main, eax

;(45)flag = 1 
	mov eax, literal_7
	mov flag_main, al

;(47)wlock = warlock 
	push offset literal_28
	push lengthof literal_28
	cld
	pop ecx
	pop esi
	lea edi, wlock_main
	rep movsb

;(48)mtama = murlakatama 
	push offset literal_30
	push lengthof literal_30
	cld
	pop ecx
	pop esi
	lea edi, mtama_main
	rep movsb

;(49)hello = hello 
	push offset literal_32
	push lengthof literal_32
	cld
	pop ecx
	pop esi
	lea edi, hello_main
	rep movsb

;(50)space =   
	push offset literal_34
	push lengthof literal_34
	cld
	pop ecx
	pop esi
	lea edi, space_main
	rep movsb

;(51)world = world 
	push offset literal_36
	push lengthof literal_36
	cld
	pop ecx
	pop esi
	lea edi, world_main
	rep movsb

;(52)t arry [ $size ] 
	xor eax, eax
	mov ebx, type arry_main
	mov ecx, offset arry_main

;(53)t arrx [ 10 ] = { 10 , 81 , 111 , 68 , 98 , 11 , 12 , 12 , 45 , 45 } 
	xor eax, eax
	mov ebx, type arrx_main
	mov ecx, offset arrx_main

;[0]=10
	mov edx, literal_26
	mov [ecx + eax], edx

;[1]=81
	add eax, ebx
	mov edx, literal_39
	mov [ecx + eax], edx

;[2]=111
	add eax, ebx
	mov edx, literal_40
	mov [ecx + eax], edx

;[3]=68
	add eax, ebx
	mov edx, literal_41
	mov [ecx + eax], edx

;[4]=98
	add eax, ebx
	mov edx, literal_42
	mov [ecx + eax], edx

;[5]=11
	add eax, ebx
	mov edx, literal_43
	mov [ecx + eax], edx

;[6]=12
	add eax, ebx
	mov edx, literal_44
	mov [ecx + eax], edx

;[7]=12
	add eax, ebx
	mov edx, literal_44
	mov [ecx + eax], edx

;[8]=45
	add eax, ebx
	mov edx, literal_45
	mov [ecx + eax], edx

;[9]=45
	add eax, ebx
	mov edx, literal_45
	mov [ecx + eax], edx

;(54) ( array long:  ) 
invoke write, ADDR literal_47
;(54)
@f5:
;(56) ( x[ , i , ]: , arrx [ i ] , , ) 
invoke write, ADDR literal_49
;(56)x[ , i , ]: , arrx [ i ] , , ) 
invoke int_to_char, ADDR output,i_main
invoke write, ADDR output
;(56)i , ]: , arrx [ i ] , , ) 
invoke write, ADDR literal_50
;(56)]: , arrx [ i ] , , ) 
;(56), arrx [ i ] , , ) 
	mov ecx, offset arrx_main
	push ecx
;(56)arrx [ i ] , , ) 
	mov eax, i_main
	pop ecx
	imul eax, type arrx_main
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
invoke int_to_char, ADDR output, eax
invoke write, ADDR output

;(56)] , , ) 
invoke write, ADDR literal_51
;(57)i ++ 
	inc i_main
	mov eax, i_main
	mov ebx, literal_20
	cmp eax, ebx	
jle @f5
;(59)i = 0 
	mov eax, literal_4
	mov i_main, eax

;(60), $size ) 
	mov eax, $size_main
	push eax
	mov ecx, offset arrx_main
	push ecx
call sort
invoke write, ADDR endl
;(62) ( array sort:  ) 
invoke write, ADDR literal_54
;(62)
@f6:
;(64) ( x[ , i , ]: , arrx [ i ] , , ) 
invoke write, ADDR literal_49
;(64)x[ , i , ]: , arrx [ i ] , , ) 
invoke int_to_char, ADDR output,i_main
invoke write, ADDR output
;(64)i , ]: , arrx [ i ] , , ) 
invoke write, ADDR literal_50
;(64)]: , arrx [ i ] , , ) 
;(64), arrx [ i ] , , ) 
	mov ecx, offset arrx_main
	push ecx
;(64)arrx [ i ] , , ) 
	mov eax, i_main
	pop ecx
	imul eax, type arrx_main
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
invoke int_to_char, ADDR output, eax
invoke write, ADDR output

;(64)] , , ) 
invoke write, ADDR literal_51
;(65)i ++ 
	inc i_main
	mov eax, i_main
	mov ebx, literal_20
	cmp eax, ebx	
jle @f6
;(67)i = 0 
	mov eax, literal_4
	mov i_main, eax

invoke write, ADDR endl
invoke write, ADDR endl
invoke write, ADDR endl
;(71) ( summ alements array =  , summarr ( arrx ) ) 
invoke write, ADDR literal_60
;(71)summ alements array =  , summarr ( arrx ) ) 
	mov ecx, offset arrx_main
	push ecx
call summarr
invoke int_to_char, ADDR output, eax
invoke write, ADDR output
invoke write, ADDR endl
invoke write, ADDR endl
;(74) ( cycle in cicle and if else ) 
invoke writeline, ADDR literal_64
;(74)
@f7:
;(76)j = i 
	mov eax, i_main
	mov j_main, eax

;(76)
@f8:
;(77){ z ( flag ) {  ( # ) 
	xor eax, eax
	mov al, flag_main
	mov ebx, 1h
	cmp eax, ebx	
jl @f9
;(80) ( # ) 
invoke write, ADDR literal_66
;(81)flag = 0 
	mov eax, literal_4
	mov flag_main, al

jmp @t9
@f9:
;(82)} e {  ( * ) 
;(83)e {  ( * ) 
;(85) ( * ) 
invoke write, ADDR literal_68
;(86)flag = 1 
	mov eax, literal_7
	mov flag_main, al

@t10:
@t9:
;(88)j ++ 
	inc j_main
	mov eax, j_main
	mov ebx, literal_20
	cmp eax, ebx	
jle @f8
;(90) ( @ ) 
invoke write, ADDR literal_70
;(91)flag = 1 
	mov eax, literal_7
	mov flag_main, al

;(92)i ++ 
	inc i_main
invoke write, ADDR endl
	mov eax, i_main
	mov ebx, literal_20
	cmp eax, ebx	
jle @f7
;(95)i = 0 
	mov eax, literal_4
	mov i_main, eax

invoke write, ADDR endl
invoke write, ADDR endl
;(98) ( fill array byte for random ) 
invoke writeline, ADDR literal_75
;(99)
@f12:
;(101)= 2 ( 0 , 255 ) 
	mov ecx, literal_77
	push ecx
	mov ecx, literal_4
	push ecx
call random

	push eax
	mov ecx, offset arry_main
	mov ebx, type arry_main
	push ebx
;(101)[ i ] = 2 ( 0 , 255 ) 
	mov eax, i_main
	pop ebx
	imul ebx
	add ecx, eax
	pop eax
	cmp eax, 0100h
	jge overflowbyte
	mov [ecx], eax
;(102)i ++ 
	inc i_main
	mov eax, i_main
	mov ebx, literal_20
	cmp eax, ebx	
jle @f12
;(104)i = 0 
	mov eax, literal_4
	mov i_main, eax

;(104)
@f13:
;(106) ( y[ , i , ]: , arry [ i ] , , ) 
invoke write, ADDR literal_79
;(106)y[ , i , ]: , arry [ i ] , , ) 
invoke int_to_char, ADDR output,i_main
invoke write, ADDR output
;(106)i , ]: , arry [ i ] , , ) 
invoke write, ADDR literal_50
;(106)]: , arry [ i ] , , ) 
;(106), arry [ i ] , , ) 
	mov ecx, offset arry_main
	push ecx
;(106)arry [ i ] , , ) 
	mov eax, i_main
	pop ecx
	xor ebx, ebx
	mov bl, al
	mov eax, ebx
	imul eax, type arry_main
	add ecx, eax
	mov edx, [ecx]
	mov eax, edx
invoke int_to_char, ADDR output, al
invoke write, ADDR output

;(106)] , , ) 
invoke write, ADDR literal_51
;(107)i ++ 
	inc i_main
	mov eax, i_main
	mov ebx, literal_20
	cmp eax, ebx	
jle @f13
invoke write, ADDR endl
invoke write, ADDR endl
;(112) ( #function call to replace characters# ) 
invoke writeline, ADDR literal_83
	mov ecx, offset mtama_main
	push ecx
	mov ecx, offset wlock_main
	push ecx
call replacestr
;(114) ( wlock ) 
invoke writeline, ADDR wlock_main
invoke write, ADDR endl
invoke write, ADDR endl
;(117)i = 10 2 2 + * 15 * 3 / 1 ( hello ) + ©©
	mov eax, literal_26
	mov ebx, literal_14
	mov ecx, literal_14
	push ecx
	pop ecx
	add ebx, ecx
	imul ebx
	mov ebx, literal_87
	imul ebx
	mov ebx, literal_88
	cmp ebx, 0
	je errordivzero
	xor edx, edx
	idiv ebx
	push eax
	mov ecx, offset hello_main
	push ecx
call strlen
	mov ebx, eax
	pop eax
	add eax, ebx
	mov i_main, eax

;(118) ( 10*(2+2)*15/3+strlen(hello) =  , i ) 
invoke write, ADDR literal_91
;(118)10*(2+2)*15/3+strlen(hello) =  , i ) 
invoke int_to_char, ADDR output,i_main
invoke write, ADDR output
invoke write, ADDR endl
invoke write, ADDR endl
;(121) ( #standart string function# ) 
invoke writeline, ADDR literal_95
;(122) ( standart strcat:  ) 
invoke write, ADDR literal_97
	mov ecx, offset space_main
	push ecx
	mov ecx, offset hello_main
	push ecx
call strcat
	mov ecx, offset world_main
	push ecx
	mov ecx, offset hello_main
	push ecx
call strcat
;(125) ( hello ) 
invoke writeline, ADDR hello_main
;(126) ( standart strlen :  , 1 ( hello ) ) 
invoke write, ADDR literal_102
;(126)standart strlen :  , 1 ( hello ) ) 
	mov ecx, offset hello_main
	push ecx
call strlen
invoke int_to_char, ADDR output, eax
invoke write, ADDR output
invoke write, ADDR endl
;(128) ( standart strcopy:  ) 
invoke write, ADDR literal_106
;(129) (  ( string , work ) ) 
	mov ecx, offset literal_110
	push ecx
	mov ecx, offset literal_109
	push ecx
call strcopy
invoke writeline, eax ;(129)
;(130)r 0 
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

end main