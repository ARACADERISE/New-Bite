section .text
	global _start
	%define system_call int 0x80
_start:

	mov ecx, age
	push ecx
	push ebp
	mov ebp, ecx

	mov ecx, age_two
	push ecx
	push ebp
	mov ebp, ecx
	system_call

	mov eax, 1
	mov ebx, 0
	system_call

section .data
	age equ 15
	age_two equ 15