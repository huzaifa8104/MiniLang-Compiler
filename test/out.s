.globl main
main:
    push %rbp
    mov %rsp, %rbp
    movq $5, %rax
    movq %rax, x(%rip)
    movq x(%rip), %rax
    push %rax
    movq $2, %rax
    movq %rax, %rbx
    pop %rax
    imulq %rbx, %rax
    movq %rax, y(%rip)
    movq y(%rip), %rax
    push %rax
    movq $3, %rax
    movq %rax, %rbx
    pop %rax
    addq %rbx, %rax
    movq %rax, %rsi
    leaq fmt(%rip), %rdi
    xor %rax, %rax
    call printf
    pop %rbp
    xor %rax, %rax
    ret

.data
x: .quad 0
y: .quad 0
fmt: .string "%d\n"
