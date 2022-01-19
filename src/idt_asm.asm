[bits 32]

global isr_stub_table, load_idt_asm
extern interrupt_handler 

%macro no_error_code_interrupt_handler 1
interrupt_handler_%+%1:
    push dword 0        ;Push zero instead of the error code
    push dword %1       ;Push the interrupt number
    jmp common_interrupt_handler        ;jump to the common interrupt handler
%endmacro

%macro error_code_interrupt_handler 1
interrupt_handler_%+%1:
    push dword %1       ;Push the interrupt number
    jmp common_interrupt_handler        ;jump to the common interrupt handler
%endmacro

section .bss:
    idt_des: resb 6

section .text:
;load_idt_instuction
;loads the IDT into the cpu
;stack: [esp + 8] address of the IDT
;       [esp + 4] size of the IDT
;       [  esp  ] returns address
load_idt_asm:
    mov eax, [esp + 4]  ;load the base address
    mov [idt_des + 2], eax
    mov ax, [esp + 8]   ;load the limit
    mov [idt_des], ax
    mov eax, idt_des
    cli                 ;clear interrupts
    lidt [eax]          ;load the gdt into memory
    ret

common_interrupt_handler:
    ;Push the cpu state
    push esp
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax

    call interrupt_handler          ;Calls the interrupt handler in C

    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp
    pop esp

    add esp, 8      ;restore the stack state

    iret        ;return to the code

no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1
no_error_code_interrupt_handler 2
no_error_code_interrupt_handler 3
no_error_code_interrupt_handler 4
no_error_code_interrupt_handler 5
no_error_code_interrupt_handler 6
no_error_code_interrupt_handler 7
error_code_interrupt_handler    8
no_error_code_interrupt_handler 9
error_code_interrupt_handler    10
error_code_interrupt_handler    11
error_code_interrupt_handler    12
error_code_interrupt_handler    13
error_code_interrupt_handler    14
no_error_code_interrupt_handler 15
no_error_code_interrupt_handler 16
error_code_interrupt_handler    17
no_error_code_interrupt_handler 18
no_error_code_interrupt_handler 19
no_error_code_interrupt_handler 20
no_error_code_interrupt_handler 21
no_error_code_interrupt_handler 22
no_error_code_interrupt_handler 23
no_error_code_interrupt_handler 24
no_error_code_interrupt_handler 25
no_error_code_interrupt_handler 26
no_error_code_interrupt_handler 27
no_error_code_interrupt_handler 28
no_error_code_interrupt_handler 29
error_code_interrupt_handler    30
no_error_code_interrupt_handler 31

section .data
isr_stub_table:     ;list of all the interrupt handler addresses
%assign i 0
%rep 32
    dd interrupt_handler_%+i
%assign i i+1
%endrep