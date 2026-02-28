        .intel_syntax noprefix
        .global main
main:
	call readi64
	mov r12, rax

	bsr r13, r12

	mov rdi, r13
	call writei64

	mov rdi, r13
	call writei64

	call finish
