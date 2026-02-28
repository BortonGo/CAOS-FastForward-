        .intel_syntax noprefix
        .global main
main:
	mov r12, 1
	
loop:
	call readi64
	cmp rax, -1
	je done

	imul r12, rax
	jmp loop

done:
	mov rdi, r12
	call writei64
	call finish
