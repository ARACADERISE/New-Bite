section .text:
	global _start
	%define system_call int 0x80

_start:
	mov eax, 1
	mov ebx, 1
	system_call