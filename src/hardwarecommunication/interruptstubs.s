.set IRQ_BASE, 0x20 # car c'est bien de garder les 32 premieres entrées de la IDT pour les exceptions

.section .text

.extern _ZN4myos21hardwarecommunication16InterruptManager15HandleInterruptEhj
.global _ZN4myos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv


.macro HandleException num
.global _ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev
 _ZN4myos21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interrupt_number)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN4myos21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interrupt_number)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C


int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs


    pushl %esp
    push (interrupt_number)
    call _ZN4myos21hardwarecommunication16InterruptManager15HandleInterruptEhj
    add %esp, 6
    mov %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN4myos21hardwarecommunication16InterruptManager22IgnoreInterruptRequestEv:

    iret



.data
    interrupt_number: .byte 0
