global loader   ;the entry symbol for the elf
extern kmain

MAGIC_NUMBER equ 0x1BADB002                     ;define the magic number
ALIGN_MOUDLES equ 0x0                           ;multiboot flags
CHECKSUM equ -(MAGIC_NUMBER + ALIGN_MOUDLES)    ;(magic_number + flags + checksum) should equal zero

KERNEL_STACK_SIZE equ 4096  ;size of stack in bytes

section .__mbHeader             ;start of the text code section
align 4                     ;the code must be 4 byte aligned
    dd MAGIC_NUMBER
    dd ALIGN_MOUDLES
    dd CHECKSUM
section .text:
loader:                     ;the loader label, defined in the linker script
    mov esp, kernel_stack + KERNEL_STACK_SIZE   ;point esp to the end of the stack
    push ebx                                    ;push multiboot header pointer
    call kmain                                  ;call the kernel in C
.loop:
    jmp .loop               ;loop forever

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE      ;reserve stack for the kernel