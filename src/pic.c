#include "pic.h"
#include "io.h"

#define PIC1 0X20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define ICW1_ICW4	    0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	    0x10		/* Initialization - required! */
 
#define ICW4_8086	    0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	    0x10		/* Special fully nested (not) */

#define PIC_ACK 0x20

/** pic_ack:
 *  Acknowledges a PIC IRQ
 * 
 *  @param irq The IRQ number of the interrupt 
 */
void pic_ack(unsigned char irq)
{
    if(irq >= 8)
        outb(PIC2_COMMAND, PIC_ACK);
    outb(PIC1_COMMAND, PIC_ACK);
}

/** pic_init:
 *  Initializes the PIC, and maps it to certain interrupts
 *  
 *  @param offset1 offset for interrupts for master PIC
 *  @param offset2 offset for interrupts for slave PIC
 */
void pic_init(unsigned char offset1, unsigned char offset2)
{
    unsigned char mask1, mask2;
    mask1 = inb(PIC1_DATA), mask2 = inb(PIC2_DATA);     //Save the masks

    //Start initialization
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    //Set offsets
    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);
    //Tell master PIC that there is a slave
    outb(PIC1_DATA, 4);
    //Tell slave its cascade identity
    outb(PIC2_DATA, 2);

    //Set to 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    //Restore saved masks
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

/** irq_set_mask
 *  Disables certain IRQ number.
 * 
 *  @param irq The IRQ to disable
 */
void irq_set_mask(unsigned char irq)
{
    unsigned short port;
    unsigned char val;

    if(irq < 8)
    {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    val = inb(port) | (1 << irq);
    outb(port, val);
}

/** irq_clear_mask
 *  Enables certain IRQ number.
 * 
 *  @param irq The IRQ to enable
 */
void irq_clear_mask(unsigned char irq)
{
    unsigned short port;
    unsigned char val;

    if(irq < 8)
    {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    val = inb(port) & ~(1 << irq);
    outb(port, val);
}