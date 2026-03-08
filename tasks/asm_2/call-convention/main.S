        .intel_syntax noprefix
        .global main

main:
        push    rbx
        push    r12
        push    r13
        push    r14
        push    r15
        push    rbp

        call    readi64
        mov     rbx, rax
        
        call    readi64
        mov     r12, rax        # b
        
        call    readi64
        mov     r13, rax        # c
        
        call    readi64
        mov     r14, rax        # d
        
        call    readi64
        mov     r15, rax        # e
        
        call    readi64
        mov     rbp, rax        # f
        
        call    readi64
        push    rax             # x
        
        call    readi64
        mov     r10, rax        # y
        
        pop     r11             # x
        
        mov     rdi, rbx        # a
        mov     rsi, r12        # b
        mov     rdx, r13        # c
        mov     rcx, r14        # d
        mov     r8, r15         # e
        mov     r9, rbp         # f
        
        push    r10             # y (arg 8)
        push    r11             # x (arg 7)
        
        call    calculate
        
        add     rsp, 16         # clear stack from args
        mov     rdi, rax
        call    writei64
        
        pop     rbp
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     rbx
        
        call    finish
