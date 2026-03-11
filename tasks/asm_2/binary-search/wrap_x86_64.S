    .global wrap, wrap_after_call
#ifndef FUNC
#define FUNC process
#endif

wrap:
    // save registers
    mov %rbx, rbxvalue(%rip)
    mov %r12, r12value(%rip)
    mov %r13, r13value(%rip)
    mov %r14, r14value(%rip)
    mov %r15, r15value(%rip)
    mov %rbp, rbpvalue(%rip)

    // replace return address
    pop %rbx
    mov %rbx, retvalue(%rip)
    mov %rsp, rspvalue(%rip)
    lea wrapret(%rip), %rbx
    push %rbx

    // tamper detection
    mov R12(%rip), %r12
    mov R13(%rip), %r13
    mov R14(%rip), %r14
    mov R15(%rip), %r15
    mov RBX(%rip), %rbx

    jmp FUNC

wrap_after_call:
wrapret:
    cmp rspvalue(%rip), %rsp
    jnz fail
    cmp rbpvalue(%rip), %rbp
    jnz fail
    cmp R12(%rip), %r12
    jnz fail
    cmp R13(%rip), %r13
    jnz fail
    cmp R14(%rip), %r14
    jnz fail
    cmp R15(%rip), %r15
    jnz fail
    cmp RBX(%rip), %rbx
    jnz fail

    // restore return address
    mov retvalue(%rip), %rbx
    push %rbx

    // restore registers
    mov rbxvalue(%rip), %rbx
    mov r12value(%rip), %r12
    mov r13value(%rip), %r13
    mov r14value(%rip), %r14
    mov r15value(%rip), %r15

    ret

fail:
    lea  errmsg(%rip), %rdi
    call puts
    mov  $13, %rdi
    call exit

errmsg:
    .asciz "the function messed up callee-saved registers"
R12:
    .quad 0xcacacacacaca
R13:
    .quad 0xf0f0f0f0f0f0
R14:
    .quad 0xf0f0f0f0f0f0
R15:
    .quad 0xf0f0f0f0f0f0
RBX:
    .quad 0xfefefefefefe

.data
retvalue:
    .quad 0
rbxvalue:
    .quad 0
rspvalue:
    .quad 0
rbpvalue:
    .quad 0
r12value:
    .quad 0
r13value:
    .quad 0
r14value:
    .quad 0
r15value:
    .quad 0
