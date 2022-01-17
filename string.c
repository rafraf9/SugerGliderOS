#include "string.h"

/** strcpy:
 *  Copies a string from source to destination
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @returns   The destination string
 */
char* strcpy(char* des, const char* src)
{
    for(char* tmp = des;*src;*(tmp++) = *(src++));
    return des;
}
/** strncpy:
 *  Copies up to n characters from source string to destination string
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @param num The maximum number of characters to copy  
 *  @returns The destination string
 */
char* strncpy(char* des, const char* src, unsigned int num)
{
    for(unsigned int i = 0;i < num && src[i]; des[i] = src[i], ++i);
    return des;
}
/** strcat:
 *  Concatenate source string unto destination string
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @returns   The destination string
 */
char* strcat(char* des, const char* src)
{
    char* tmp = des;
    for(;*des;++des);   //Reach end of des string
    strcpy(des, src);   //Copy unto end of string

    return tmp;
}
/** strcat:
 *  Concatenate up to n characters source string unto destination string
 * 
 *  @param des The destination string
 *  @param src The source string
 *  @param num the destination buffer size
 *  @returns   The destination string
 */
char* strncat(char* des, const char* src, unsigned int num)
{
    char* tmp = des;
    for(;*des;++des, --num);   //Reach end of des string
    strncpy(des, src, num);   //Copy unto end of string

    return tmp;
}
/** memcpy:
 *  Copies n bytes from source buffer to destination buffer
 * 
 *  @param dest The destination buffer
 *  @param src  The source buffer
 *  @param num  The number of bytes to copy.
 *  @returns    The destination buffer
 */
void* memcpy(void* dest, const void* src, unsigned int num)
{
    for(unsigned int i = 0; i < num; ((char*)dest)[i] = ((const char*)src)[i], ++i);
    return dest;
}
/** memset:
 *  Fills a buffer with value c
 * 
 *  @param buf The buffer to be set
 *  @param c   The value to fill the buffer with
 *  @param num The size of the buffer
 *  @returns   buf
 */
void* memset(void* buf, int c, unsigned num)
{
    unsigned char val = (unsigned char)c;
    for(unsigned int i  = 0; i < num; ((char*)buf)[i++] = val);
    return buf;
}
/** strlen:
 *  Calculates the length of a string.
 * 
 *  @param str The string being calculated
 *  @returns The length of the string
 */
unsigned int strlen(const char* str)
{
    unsigned int size = 0;
    while(str[size++]);
    --size;
    return size;
}

/** isdigit:
 *  Takes a chacrter and checks if its a digit
 * 
 *  @param c The character were checking
 *  @returns 1 If the character is a digit, else 0
 */
int isdigit(int c)
{
    unsigned char val = (unsigned char)c;
    return val >= '0' && val <= '9';
}