#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/** outb:
*   Sends the given data to the given I/O port, Defined in io.asm
*   
*   @param port The I/O port to send the data to
*   @param data The data to send to the I/O port
*/
void outb(unsigned short port, unsigned char data);

/** inb:
 *  Gets data from the given I/O port, Defined in io.asm
 * 
 * @param port The I/O port to get data from
 * @returns The data from the I/O port
*/
unsigned char inb(unsigned short port);

/** cli
 *  Disable the interrupts.
 */
void cli(void);

/**sti
 * Enable interrupts.
 */
void sti(void);

#endif /*INCLUDE_IO_H*/