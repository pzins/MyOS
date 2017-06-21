# constant used for creating multiboot header
.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1badb002
.set CHECKSUM, -(MAGIC + FLAGS)


# declare header as in the multiboot standard
.section .multiboot
    .align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM


.section .text
.extern kernelMain
.extern callConstrutors
.global loader

loader:
    mov $kernel_stack, %esp

    call callConstrutors

    push %eax
    push %ebx
    call kernelMain


_stop:
    cli
    hlt
    jmp _stop



.section .bss
.space 2*1024*1024; #2 MiB
kernel_stack:
