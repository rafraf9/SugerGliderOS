global outb
;outb - sends a byte to an I/O port
;stack: [esp + 8] the data byte
;       [esp + 4] the I/O port
;       [esp    ] the return address
outb:
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al          ;send the data on the I/O port
    ret

global inb
;inb - gets a byte from an I/O port
;stack: [esp + 4] the I/O port
;       [esp    ] the return address
inb:
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret