.globl _start

_start:
	movl $1, %ebp
	movl $1, %eax

	push %ebp
	movl %esp, %ebp
	movl %ebp, %esp
	pop %ebp

	push %ebp
	movl %esp, %ebp
	movl %ebp, %esp
	pop %ebp

	int $0x80
	ret
