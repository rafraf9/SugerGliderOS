#ifndef PRINT_STRING_H
#define PRINT_STRING_H

#include "stdarg.h"

typedef int(*write_func)(const char* , unsigned int);

/** puts:
 *  Prints a string to the screen
 *  
 *  @param str the string to print to the screen 
 *  @returns 1 if succesfull, 0 if not
*/
int puts(const char* str);
/** putsw:
 *  print string through a driver
 * 
 *  @param str      The string to print in the driver
 *  @param write    The driver write function to use
 *  @returns 1 if succesfull, 0 if not
 */
int putsw(const char* str, write_func write);
/** putchar:
 *  prints a single character to the screen.
 * 
 *  @param c The character being printed
 *  @returns The character printed, or -1 on error
 */
int putchar(int c);
/** putchar:
 *  prints a single character through a driver.
 * 
 *  @param c The character being sent to the driver.
 *  @param write    The driver write function to use
 *  @returns The character printed, or -1 on error
 */
int putcharw(int c, write_func write);

//Credit to User:A22347 for the printf example that was sloghtly modified for this.

/** vprintf:
 *  Sends a formatted string to a driver
 *  
 *  @param format The formatted string to send
 *  @param list   The list of arguments for formatted values
 *  @param write  The driver write function to use
 *  @returns On success returns the number of characters written, else a negetive number is returned
 */
int vprintf (const char* format, va_list list, write_func write);

/** printf:
 *  Prints a formatted string to the screen
 *  
 *  @param format The formatted string to print
 *  @returns On success returns the number of characters written, else a negetive number is returned
 */
__attribute__ ((format (printf, 1, 2))) int printf (const char* format, ...);

/** printf:
 *  Sends a formatted string to a driver
 *  
 *  @param format The formatted string to send
 *  @param write The driver write function to use
 *  @returns On success returns the number of characters written, else a negetive number is returned
 */
__attribute__ ((format (printf, 1, 3))) int printfw (const char* format, write_func write, ...);

#endif /*PRINT_STRING_H*/