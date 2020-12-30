.586p
.model flat, stdcall
.stack 4096
;----------------------PROTO---------------------
GetStdHandle			PROTO : DWORD
WriteConsoleA			PROTO : DWORD, : DWORD, : DWORD, : DWORD, : DWORD
MessageBoxA  			PROTO : DWORD, : DWORD, : DWORD, : DWORD
;----------------------DATA----------------------
.DATA
endl				dword	0Ah, 0
;----------------------CODE----------------------
.CODE
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
END