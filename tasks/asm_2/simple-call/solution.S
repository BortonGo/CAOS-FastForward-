	.intel_syntax noprefix
	.global main
	.extern func
	.extern printf

	.section .rodata
fmt:
    	.string "%d\n"

	.section .text
main:
    	sub rsp, 8

    	mov edi, 5
    	mov esi, 2
    	call func

    	mov esi, eax
    	lea rdi, [rip + fmt]
    	xor eax, eax
    	call printf

    	xor eax, eax
    	add rsp, 8
   	 ret

.section .note.GNU-stack,"",@progbits

