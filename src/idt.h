#ifndef IDT_H
#define IDT_H

typedef void (*irq_handler)(unsigned char irq);

/** idt_init:
 *  Initialized the IDT to handle interrupts
 */
void idt_init(void);

/** register_irq_handler:
 *  Register an IRQ handler to a specific interrupt
 *  
 *  @param irq The IRQ number to register the handler
 *  @param handler The IRQ handler to be registered
 */
void register_irq_handler(unsigned char irq, irq_handler handler);

/** unregister_irq_handler
 *  Unregister an IRQ handler to a specific interrupt
 * 
 *  @param irq The IRQ number to unregister
 */
void unregister_irq_handler(unsigned char irq);

#endif /* IDT_H */