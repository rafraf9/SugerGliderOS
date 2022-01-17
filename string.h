#ifndef STRING_H
#define STRING_H

/** strcpy:
 *  Copies a string from source to destination
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @returns   The destination string
 */
char* strcpy(char* des, const char* src);
/** strncpy:
 *  Copies up to n characters from source string to destination string
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @param num The maximum number of characters to copy  
 *  @returns The destination string
 */
char* strncpy(char* des, const char* src, unsigned int num);
/** strcat:
 *  Concatenate source string unto destination string
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @returns   The destination string
 */
char* strcat(char* des, const char* src);
/** strcat:
 *  Concatenate up to n characters source string unto destination string
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @param num the destination buffer size
 *  @returns   The destination string
 */
char* strncat(char* des, const char* src, unsigned int num);
/** memcpy:
 *  Copies n bytes from source buffer to destination buffer
 * 
 *  @param dest The destination buffer
 *  @param src  The source buffer
 *  @param num  The number of bytes to copy.
 *  @returns    The destination buffer
 */
void* memcpy(void* dest, const void* src, unsigned int num);
/** memset:
 *  Fills a buffer with value c
 * 
 *  @param buf The buffer to be set
 *  @param c   The value to fill the buffer with
 *  @param num The size of the buffer
 *  @returns   buf
 */
void* memset(void* buf, int c, unsigned num);
/** strlen:
 *  Calculates the length of a string.
 * 
 *  @param str The string being calculated
 *  @returns The length of the string
 */
unsigned int strlen(const char* str);

/** isdigit:
 *  Takes a chacrter and checks if its a digit
 * 
 *  @param c The character were checking
 *  @returns 1 If the character is a digit, else 0
 */
int isdigit(int c);

#endif /*STRING_H*/