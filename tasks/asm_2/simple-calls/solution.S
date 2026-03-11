        .intel_syntax noprefix
        .global func
        .text
func:
	mov rax, rdi
	add rax, rsi
	add rax, rdx
	cqo
	mov rcx, 3
	idiv rcx
        ret
