#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

/** serial_write:
 *  Writes a buffer of length len to the serial port
 * 
 *  @param buf The buffer being writen to the serial port
 *  @param len The length of the buffer being written
 *  @returns The number of characters written to the serial port, returns -1 if failed
*/
int serial_write(const char* buf, unsigned int len);

#endif /*SERIAL_DRIVER_H*/