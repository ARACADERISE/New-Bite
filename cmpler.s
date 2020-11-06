section .text
	global _start
	global main
	global print
	%define system_call int 0x80
main:

	mov ecx, age
	system_call

	mov ecx, age_two
	system_call

	mov ecx, anotherAge
	system_call

	call print

	mov eax, 1
	mov ebx, 0
	system_call


_start:

	push ebp
	mov ebp, esp
	sub esp, 16
	system_call

	call main
	pop ebp
	mov esp, ebp
	system_call

	mov eax, 1
	mov ebx, 0
	system_call

print:

	mov eax, '22'
	mov [var], eax
	mov ecx, var
	mov edx, 2
	mov eax, 4
	mov ebx, 1
	system_call

	mov eax, [anotherAge]
	mov [var], eax
	mov ecx, var
	mov edx, 3
	mov eax, 4
	mov ebx, 1
	system_call

	mov eax, 1
	mov ebx, 0
	system_call

segment .bss
	var resb 4
section .rodata
	age db '155'
	age_two db '155'
	anotherAge db '155'