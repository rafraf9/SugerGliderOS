#include "idt.h"
#include "print_string.h"
#include "breakpoint.h"

typedef struct
{
    unsigned short offset_low;      //The lower 16 bits of the isr handler address
    unsigned short segment_sel;     //The segment selector for the isr
    unsigned char reserved;         //Should be zero (at least 3 top bits)
    unsigned char attribs;          //type of gate(16 or 32), privelage level, is the hanlder present
                                    //in memory
    unsigned short offset_high;     //The top 16 bits of the isr hadler address
} __attribute__((packed)) IDT_entry;

typedef struct
{
    unsigned int address;
    unsigned short size;
} __attribute__((packed)) IDTR;


typedef struct
{
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    unsigned int ebp;
    unsigned int esp;
} __attribute__((packed)) cpu_state;

typedef struct
{
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed)) stack_state;

extern void* isr_stub_table[];      //List of isr address stubs
static IDTR idtr;                   //The idt table register

__attribute__((aligned(0x10)))      //List of IDT entries, aligned for performance      
static IDT_entry idt[256];

/** load_idt_asm:
 *  Calls assembly code to load the IDT
 * 
 *  @param idtr pointer to IDT descriptor
 */
void load_idt_asm(IDTR idtr);

/** idt_set_descriptor:
 *  Sets the specified interrupt vector to a function, with flags
 * 
 *  @param vector The vector to set
 *  @param isr    The memory address to be the isr
 *  @param flags  The flags for the idt entry
 */
void idt_set_descriptor(unsigned char vector, void* isr, unsigned char flags)
{
    IDT_entry* descriptor = &idt[vector];

    descriptor->offset_low = ((unsigned int)isr) & 0xFFFF;
    descriptor->segment_sel = 0x08; //Choose the kernel segment selector
    descriptor->attribs = flags;
    descriptor->offset_high = ((unsigned int)isr) >> 16;
    descriptor->reserved = 0;
}

/** idt_init:
 *  Initialized the IDT to handle interrupts
 */
void idt_init(void)
{
    idtr.address = (unsigned int)(&idt[0]);
    idtr.size = sizeof(idt[0]) * sizeof(idt);

    for(unsigned char i = 0; i < 32; ++i)
    {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);        //Set to kernel level interrupt
    }

    load_idt_asm(idtr);     //Load the new IDT
}

/** interrupt_handler:
 *  A general handler for all interrupts
 *  
 *  @param regs The state the cpu was in
 *  @param stack The original stack state
 *  @param interrupt The interrupt number 
 */
void interrupt_handler(cpu_state regs, unsigned int interrupt, stack_state stack)
{
    printf("Recieved interrupt %d\n", interrupt);
    printf("registers: %d %d %d %d %d %d %d %d\n", regs.eax, regs.ebx, 
    regs.ecx, regs.edx, regs.esi, regs.edi, regs.esp, regs.ebp);
    printf("error code: %d\n", stack.error_code);
    printf("address %d:%d\n", stack.cs, stack.eip);
    printf("eflags: %d\n", stack.eflags);
    BREAK;
}