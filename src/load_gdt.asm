[bits 32]

global load_gdt_asm

section .bss:
    gdt_des: resb 6

section .text:
;load_gdt_instuction
;loads the gdt into the cpu
;stack: [esp + 8] address of the GDT
;       [esp + 4] size of the gdt
;       [  esp  ] returns address

load_gdt_asm:
    xchg bx, bx
    mov eax, [esp + 4]  ;load the base address
    mov [gdt_des + 2], eax
    mov ax, [esp + 8]   ;load the limit
    mov [gdt_des], ax
    mov eax, gdt_des
    cli                 ;clear interrupts
    lgdt [eax]          ;load the gdt into memory

    jmp 0x08:.reset_cs  ;reload code segment register
.reset_cs:
    ;reload the data registers
    mov ax, 0x10
    mov dx, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret