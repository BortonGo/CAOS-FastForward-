        .intel_syntax noprefix
        .global main
main:
	call readi64
	mov r12, rax

loop: 
	call readi64
	mov r13, rax

loop_sec:
	call readi64
	mov r14, rax
	cmp r14, -1
	jz out
	cmp r13, r14
	jge skip
	mov r13, r14
skip:
	jmp loop_sec

out:
	mov rdi, r13
	call writei64
	dec r12
	jnz loop
	call finish
