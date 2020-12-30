.586p
.model flat, stdcall
.stack 4096
;--------------- CONST --------------;
.CONST
str_end				EQU		0
;----------------------PROTO---------------------
write					PROTO : DWORD
writeline				PROTO : DWORD
ExitProcess  			PROTO : DWORD
;----------------------DATA----------------------
.DATA
;-------------- SYSTEM ID ERROR --------------;
errorid1				DWORD	  1
errorid2				DWORD	  2
errorid3				DWORD	  3
errorid4				DWORD	  4
;--------------- SYSTEM ERROR ---------------;
erroverflow 			BYTE      "Error Integer overflow",str_end
errdivbyzero			BYTE      "Integer division by zero",str_end
errrandomefunc			BYTE      "Error parametres randome function second less first or equals",str_end
erroverflowbyte  		BYTE      "Error byte overflow",str_end
;----------------------CODE----------------------
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
END