        .intel_syntax noprefix
        .global calculate

calculate:
        push rbp
	mov rbp, rsp

	mov rax, rdi
	add rax, rsi
	add rax, rdx
	add rax, rcx
	add rax, r8
	add rax, r9

	mov r10, [rbp+24]
	add r10, 1

	imul r10, [rbp+16]

	add rax, r10
	pop rbp
        ret
