global loader   ;the entry symbol for the elf

MAGIC_NUMBER equ 0x1BADB002 ;define the magic number
FLAGS equ 0x0               ;multiboot flags
CHECKSUM equ -MAGIC_NUMBER  ;(magic_number + flags + checksum) should equal zero

section .text:              ;start of the text code section
align 4                     ;the code must be 4 byte aligned
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

loader:                     ;the loader label, defined in the linker script
    mov eax, 0xcafebabe     ;place the number in register eax
.loop:
    jmp .loop               ;loop forever