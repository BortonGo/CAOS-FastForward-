	.intel_syntax noprefix
	.global main

	.extern getchar
	.extern puts

	.section .rodata
yes_str:
    .asciz "YES"
no_str:
    .asciz "NO"

	.section .text
main:
    	push rbp
    	mov rbp, rsp
    	sub rsp, 1024                

    	lea rdi, [rbp - 1024]        
    	xor eax, eax                 
    	mov ecx, 256                
    	rep stosd                    

read_first:
    	call getchar                 
    	cmp eax, 10                  
    	je read_second_start

    	cmp eax, 32                  
    	je read_first

    	cmp eax, 'A'
    	jl first_count
    	cmp eax, 'Z'
    	jg first_count
    	add eax, 32

first_count:
    	add dword ptr [rbp - 1024 + rax*4], 1
    	jmp read_first

read_second_start:
read_second:
    	call getchar
    	cmp eax, 10
    	je check_freq

    	cmp eax, 32
    	je read_second

    	cmp eax, 'A'
    	jl second_count
    	cmp eax, 'Z'
    	jg second_count
    	add eax, 32

second_count:
    	sub dword ptr [rbp - 1024 + rax*4], 1
    	jmp read_second

check_freq:
    	xor ecx, ecx                 # i = 0

check_loop:
    	cmp ecx, 256
    	je print_yes

    	cmp dword ptr [rbp - 1024 + rcx*4], 0
    	jne print_no

    	inc ecx
    	jmp check_loop

print_yes:
    	lea rdi, [rip + yes_str]
    	call puts
    	xor eax, eax
    	leave
    	ret

print_no:
    	lea rdi, [rip + no_str]
    	call puts
    	xor eax, eax
    	leave
    	ret
