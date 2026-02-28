        .intel_syntax noprefix
        .global main
main:
	call readi64
    	mov r12, rax          
    	call readi64
    	mov r13, rax          

    
    	cmp r13, 0
    	jge p_nonneg
    	cmp r12, 1
    	je  out_one
    	xor r14, r14         
    	jmp out

p_nonneg:
    	mov r14, 1            
    	test r13, r13
    	jz out                

pow_loop:
    	test r13, 1           
    	jz   skip_mul
    	imul r14, r12         
skip_mul:
   	imul r12, r12         
   	shr  r13, 1           
    	jnz  pow_loop

out:
    	mov rdi, r14
    	call writei64
    	call finish

out_one:
    	mov r14, 1
    	jmp out
