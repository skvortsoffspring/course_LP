;--08-12-2020 --вторник-- 19:04:22 --
;-------------ASM-------------
.686p
.XMM
.model flat, C
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