section .text
	global _start
	%define system_call int 0x80

_start:

	mov ecx, age
	system_call

	mov ecx, age_two
	system_call

	mov eax, 1
	mov ebx, 0
	system_call

segment .data
	age dw '15'
	age_two dw '15'