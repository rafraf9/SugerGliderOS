#include "logging.h"
#include "serial_driver.h"
#include "print_string.h"

/** log_debug:
 * Writes a debug message to serial output
 * 
 * @param fmt The formatted message to write
 */
void log_debug(const char* fmt, ...)
{
    putsw("[DEBUG] ", serial_write);
    va_list list;
    va_start (list, fmt);
    vprintf (fmt, list, serial_write);
    putcharw('\n', serial_write);
    va_end (list);
}

/** log_info:
 * Writes an info message to serial output
 * 
 * @param fmt The formatted message to write
 */
void log_info(const char* fmt, ...)
{
    putsw("[INFO] ", serial_write);
    va_list list;
    va_start (list, fmt);
    vprintf (fmt, list, serial_write);
    putcharw('\n', serial_write);
    va_end (list);
}

/** log_error:
 * Writes an error message to serial output
 * 
 * @param fmt The formatted message to write
 */
void log_error(const char* fmt, ...)
{
    putsw("[ERROR] ", serial_write);
    va_list list;
    va_start (list, fmt);
    vprintf (fmt, list, serial_write);
    putcharw('\n', serial_write);
    va_end (list);
}

