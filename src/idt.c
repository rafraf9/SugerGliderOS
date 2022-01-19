#include "idt.h"
#include "print_string.h"
#include "breakpoint.h"
#include "pic.h"

#define IRQ_START   0X20
#define IRQ_SIZE    0x10

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

//List of IRQ handlers
static irq_handler irq_handlers[256];

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

    //Initialize the PIC
    pic_init(IRQ_START, IRQ_START + 7);
    irq_set_mask(0);        //For now, disable the timer.

    //Set the IRQs
    for(unsigned char i = IRQ_START; i < IRQ_START + IRQ_SIZE; ++i)
    {
        idt_set_descriptor(i, isr_stub_table[i], 0x8E);
        pic_ack(i - IRQ_START); //Just in case it has a "unACKed" IRQ from the bootloader.
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
void interrupt_handler(__attribute__((unused)) cpu_state regs, unsigned int interrupt, 
    __attribute__((unused)) stack_state stack)
{
    if(interrupt >= IRQ_START && interrupt < IRQ_START + IRQ_SIZE)
    {
        pic_ack(interrupt - IRQ_START);
        if(irq_handlers[interrupt - IRQ_START])
        {
            irq_handlers[interrupt - IRQ_START](interrupt - IRQ_START);
        }
    }
}

/** register_irq_handler:
 *  Register an IRQ handler to a specific interrupt
 *  
 *  @param irq The IRQ number to register the handler
 *  @param handler The IRQ handler to be registered
 */
void register_irq_handler(unsigned char irq, irq_handler handler)
{
    irq_handlers[irq] = handler;
}

/** unregister_irq_handler
 *  Unregister an IRQ handler to a specific interrupt
 * 
 *  @param irq The IRQ number to unregister
 */
void unregister_irq_handler(unsigned char irq)
{
    irq_handlers[irq] = 0x0;
}