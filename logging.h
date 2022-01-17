#ifndef LOGGING_H
#define LOGGING_H

/** log_debug:
 * Writes a debug message to serial output
 * 
 * @param fmt The formatted message to write
 */
void log_debug(const char* fmt, ...);

/** log_info:
 * Writes an info message to serial output
 * 
 * @param fmt The formatted message to write
 */
void log_info(const char* fmt, ...);

/** log_error:
 * Writes an error message to serial output
 * 
 * @param fmt The formatted message to write
 */
void log_error(const char* fmt, ...);

#endif /* LOGGING_H */