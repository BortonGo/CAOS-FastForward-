        .intel_syntax noprefix
        .global main
main:
	call readi64
	mov r12, rax
	call readi64
	add r12, rax
	mov rdi, r12
	call writei64
	call finish
