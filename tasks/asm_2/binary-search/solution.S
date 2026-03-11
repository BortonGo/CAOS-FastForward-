.intel_syntax noprefix
.global bsearch40

.section .text

bsearch40:
    push rbp
    mov rbp, rsp

    test rsi, rsi
    jz not_found

    mov r8, rdi
    xor rcx, rcx
    mov rdx, rsi
    dec rdx

loop_bs:
    cmp rcx, rdx
    jg not_found

    mov rax, rdx
    sub rax, rcx
    shr rax, 1
    add rax, rcx

    mov r9d, DWORD PTR [r8 + rax*4]

    test r9d, 1
    jnz go_right

    cmp r9d, 40
    je found
    jl go_right

    lea rdx, [rax - 1]
    jmp loop_bs

go_right:
    lea rcx, [rax + 1]
    jmp loop_bs

found:
    leave
    ret

not_found:
    mov eax, -1
    leave
    ret
