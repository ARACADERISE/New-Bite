section .text
	global _start
	global err
	global main
	global print
	%define system_call int 0x80

;This will be used if there is a error captured during
;compilation
err:

	mov eax, 1
	mov ebx, 1
	system_call
main:

	mov ecx, bruh
	system_call

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

section .rodata
	msg db "",0xa
	len equ $ - msg

	bruh db '553'