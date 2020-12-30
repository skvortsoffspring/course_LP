.586p
.model flat, stdcall
.stack 4096
;----------------------PROTO---------------------
@error					PROTO : DWORD
;----------------------DATA----------------------
.DATA
count					DWORD	 0
seed					DWORD	 0
errorid3				DWORD	 3
;----------------------CODE----------------------
.CODE
random  proc,
@min : DWORD,
@max : DWORD
	mov		eax, @min
	cmp		eax, @max
	jae errorparametres
	mov		eax, [seed]
	or		eax, eax
	jnz	@2
@1:
RDTSC
	or		eax, eax
	jz @1
@2:
	xor		edx, edx
	mov		ebx, 127773
	div		ebx
	push	eax
	mov		eax, 16807
	mul		edx
	pop		edx
	push	eax
	mov		eax, 2836
	mul		edx
	pop		edx
	sub		edx, eax
	mov		eax, edx
	mov		[seed], edx
	xor		edx, edx
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
random endp
END