.586p
.model flat, stdcall
.stack 4096
;----------------------PROTO---------------------
;----------------------DATA----------------------
.DATA
count					dword 0
;----------------------CODE----------------------
.CODE
strcopy proc  pfirststr : DWORD, 	psecondstr : DWORD
	xor eax, eax
	mov ecx, pfirststr
	mov edx, psecondstr
@copy:
	mov al, [edx]
	mov [ecx], al	
	inc ecx
	inc edx
	cmp	al, 0
jne @copy
	mov eax, pfirststr
	ret
jne @copy
strcopy endp
strcat proc pfirststr : DWORD, 	psecondstr : DWORD
	xor eax, eax
	mov ecx, pfirststr
	mov edx, psecondstr
@endfirst:
	inc ecx
	inc count					;lenght string
	mov al, [ecx]
	cmp al, 0
jne @endfirst
@copy:
	mov bl, [edx]
	mov [ecx], bl	
	mov al, [edx]
	inc count
	cmp count, 0ffh
je endstrsize
	inc ecx
	inc edx
	cmp	al,0
jne @copy
endstrsize:
	mov eax, pfirststr
	ret
strcat ENDP
strlen proc, ptrstr : DWORD
	xor eax, eax
	xor ebx, ebx
	mov ecx, -1
	mov edx, ptrstr
@repeat:
	inc ecx
	mov al, [edx+ecx]
	cmp al, 0
jne @repeat
	mov eax, ecx
	ret
strlen ENDP
END