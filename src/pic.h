#ifndef PIC_H
#define PIC_H
/** pic_ack:
 *  Acknowledges a PIC IRQ
 * 
 *  @param irq The IRQ number of the interrupt 
 */
void pic_ack(unsigned char irq);

/** pic_init:
 *  Initializes the PIC, and maps it to certain interrupts
 *  
 *  @param offset1 offset for interrupts for master PIC
 *  @param offset2 offset for interrupts for slave PIC
 */
void pic_init(unsigned char offset1, unsigned char offset2);

/** irq_set_mask
 *  Disables certain IRQ number.
 * 
 *  @param irq The IRQ to disable
 */
void irq_set_mask(unsigned char irq);

/** irq_clear_mask
 *  Enables certain IRQ number.
 * 
 *  @param irq The IRQ to enable
 */
void irq_clear_mask(unsigned char irq);
#endif /* PIC_H */