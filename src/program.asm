[bits 32]
;Just a simple program to see if we can load programs
section .text:
start:
    mov eax, 0xDEADBEEF     ;Test Value to see that the program has run

    jmp $                   ;Infinate loop