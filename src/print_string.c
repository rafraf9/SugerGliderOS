#include "print_string.h"
#include "fb_driver.h"
#include "string.h"
#include "stddef.h"

typedef enum {false = 0, true = 1} bool;

/** puts:
 *  Prints a string to the screen
 *  
 *  @param str the string to print to the screen 
 *  @returns 1 if succesfull, 0 if not
*/
int puts(const char* str)
{
    return putsw(str, fb_write);
}
/** putsw:
 *  print string through a driver
 * 
 *  @param str      The string to print in the driver
 *  @param write    The driver write function to use
 *  @returns 1 if succesfull, 0 if not
 */
int putsw(const char* str, write_func write)
{
    unsigned int str_size = strlen(str);
    return write(str, str_size);
}
/** putchar:
 *  prints a single character to the screen.
 * 
 *  @param c The character being printed
 *  @returns The character printed, or -1 on error
 */
int putchar(int c)
{
    return putcharw(c, fb_write);
}
/** putchar:
 *  prints a single character through a driver.
 * 
 *  @param c The character being sent to the driver.
 *  @param write    The driver write function to use
 *  @returns The character printed, or -1 on error
 */
int putcharw(int c, write_func write)
{
    char val = (unsigned char)c;

    if(write(&val, 1))
        return val;
    return -1;
}

char* __int_str(long int i, char b[], int base, bool plusSignIfNeeded, bool spaceSignIfNeeded,
                int paddingNo, bool justify, bool zeroPad) {
 
    char digit[32] = {0};
    memset(digit, 0, 32);
    strcpy(digit, "0123456789");
 
    if (base == 16) {
        strcat(digit, "ABCDEF");
    } else if (base == 17) {
        strcat(digit, "abcdef");
        base = 16;
    }
 
    char* p = b;
    if (i < 0) {
        *p++ = '-';
        i *= -1;
    } else if (plusSignIfNeeded) {
        *p++ = '+';
    } else if (!plusSignIfNeeded && spaceSignIfNeeded) {
        *p++ = ' ';
    }
 
    long int shifter = i;
    do {
        ++p;
        shifter = shifter / base;
    } while (shifter);
 
    *p = '\0';
    do {
        *--p = digit[i % base];
        i = i / base;
    } while (i);
 
    int padding = paddingNo - (int)strlen(b);
    if (padding < 0) padding = 0;
 
    if (justify) {
        while (padding--) {
            if (zeroPad) {
                b[strlen(b)] = '0';
            } else {
                b[strlen(b)] = ' ';
            }
        }
 
    } else {
        char a[256] = {0};
        while (padding--) {
            if (zeroPad) {
                a[strlen(a)] = '0';
            } else {
                a[strlen(a)] = ' ';
            }
        }
        strcat(a, b);
        strcpy(b, a);
    }
 
    return b;
}
 
void displayCharacter(char c, int* a, write_func write) {
    putcharw(c, write);
    *a += 1;
}
 
void displayString(char* c, int* a, write_func write) {
    for (int i = 0; c[i]; ++i) {
        displayCharacter(c[i], a, write);
    }
}

/** vprintf:
 *  Sends a formatted string to a driver
 *  
 *  @param format The formatted string to send
 *  @param list   The list of arguments for formatted values
 *  @param write  The driver write function to use
 *  @returns On success returns the number of characters written, else a negetive number is returned
 */
int vprintf (const char* format, va_list list, write_func write)
{
    int chars        = 0;
    char intStrBuffer[256] = {0};
 
    for (int i = 0; format[i]; ++i) {
 
        char specifier   = '\0';
        char length      = '\0';
 
        int  lengthSpec  = 0; 
        int  precSpec    = 0;
        bool leftJustify = false;
        bool zeroPad     = false;
        bool spaceNoSign = false;
        bool altForm     = false;
        bool plusSign    = false;
        bool emode       = false;
        int  expo        = 0;
 
        if (format[i] == '%') {
            ++i;
 
            bool extBreak = false;
            while (1) {
 
                switch (format[i]) {
                    case '-':
                        leftJustify = true;
                        ++i;
                        break;
 
                    case '+':
                        plusSign = true;
                        ++i;
                        break;
 
                    case '#':
                        altForm = true;
                        ++i;
                        break;
 
                    case ' ':
                        spaceNoSign = true;
                        ++i;
                        break;
 
                    case '0':
                        zeroPad = true;
                        ++i;
                        break;
 
                    default:
                        extBreak = true;
                        break;
                }
 
                if (extBreak) break;
            }
 
            while (isdigit(format[i])) {
                lengthSpec *= 10;
                lengthSpec += format[i] - 48;
                ++i;
            }
 
            if (format[i] == '*') {
                lengthSpec = va_arg(list, int);
                ++i;
            }
 
            if (format[i] == '.') {
                ++i;
                while (isdigit(format[i])) {
                    precSpec *= 10;
                    precSpec += format[i] - 48;
                    ++i;
                }
 
                if (format[i] == '*') {
                    precSpec = va_arg(list, int);
                    ++i;
                }
            } else {
                precSpec = 6;
            }
 
            if (format[i] == 'h' || format[i] == 'l' || format[i] == 'j' ||
                   format[i] == 'z' || format[i] == 't' || format[i] == 'L') {
                length = format[i];
                ++i;
                if (format[i] == 'h') {
                    length = 'H';
                } else if (format[i] == 'l') {
                    length = 'q';
                    ++i;
                }
            }
            specifier = format[i];
 
            memset(intStrBuffer, 0, 256);
 
            int base = 10;
            if (specifier == 'o') {
                base = 8;
                specifier = 'u';
                if (altForm) {
                    displayString("0", &chars, write);
                }
            }
            if (specifier == 'p') {
                base = 16;
                length = 'z';
                specifier = 'u';
            }
            switch (specifier) {
                case 'X':
                    base = 16;
                    /* fall through */
                case 'x':
                    base = base == 10 ? 17 : base;
                    if (altForm) {
                        displayString("0x", &chars, write);
                    }
                    /* fall through */
 
                case 'u':
                {
                    switch (length) {
                        case 0:
                        {
                            unsigned int integer = va_arg(list, unsigned int);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        case 'H':
                        {
                            unsigned char integer = (unsigned char) va_arg(list, unsigned int);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        case 'h':
                        {
                            unsigned short int integer = va_arg(list, unsigned int);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        case 'l':
                        {
                            unsigned long integer = va_arg(list, unsigned long);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        case 'q':
                        {
                            unsigned long long integer = va_arg(list, unsigned long long);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        case 'j':
                        {
                            unsigned long integer = va_arg(list, unsigned long);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        case 'z':
                        {
                            size_t integer = va_arg(list, size_t);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        case 't':
                        {
                            ptrdiff_t integer = va_arg(list, ptrdiff_t);
                            __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                            displayString(intStrBuffer, &chars, write);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
 
                case 'd':
                case 'i':
                {
                    switch (length) {
                    case 0:
                    {
                        int integer = va_arg(list, int);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    case 'H':
                    {
                        signed char integer = (signed char) va_arg(list, int);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    case 'h':
                    {
                        short int integer = va_arg(list, int);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    case 'l':
                    {
                        long integer = va_arg(list, long);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    case 'q':
                    {
                        long long integer = va_arg(list, long long);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    case 'j':
                    {
                        long integer = va_arg(list, long);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    case 'z':
                    {
                        size_t integer = va_arg(list, size_t);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    case 't':
                    {
                        ptrdiff_t integer = va_arg(list, ptrdiff_t);
                        __int_str(integer, intStrBuffer, base, plusSign, spaceNoSign, lengthSpec, leftJustify, zeroPad);
                        displayString(intStrBuffer, &chars, write);
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }
 
                case 'c':
                {
                    if (length == 'l') {
                        displayCharacter(va_arg(list, int), &chars, write);
                    } else {
                        displayCharacter(va_arg(list, int), &chars, write);
                    }
 
                    break;
                }
 
                case 's':
                {
                    displayString(va_arg(list, char*), &chars, write);
                    break;
                }
 
                case 'n':
                {
                    switch (length) {
                        case 'H':
                            *(va_arg(list, signed char*)) = chars;
                            break;
                        case 'h':
                            *(va_arg(list, short int*)) = chars;
                            break;
 
                        case 0: {
                            int* a = va_arg(list, int*);
                            *a = chars;
                            break;
                        }
 
                        case 'l':
                            *(va_arg(list, long*)) = chars;
                            break;
                        case 'q':
                            *(va_arg(list, long long*)) = chars;
                            break;
                        case 'j':
                            *(va_arg(list, long*)) = chars;
                            break;
                        case 'z':
                            *(va_arg(list, size_t*)) = chars;
                            break;
                        case 't':
                            *(va_arg(list, ptrdiff_t*)) = chars;
                            break;
                        default:
                            break;
                    }
                    break;
                }
 
                case 'e':
                case 'E':
                    emode = true;
                    /* fall through */
                case 'f':
                case 'F':
                case 'g':
                case 'G':
                {
                    double floating = va_arg(list, double);
 
                    while (emode && floating >= 10) {
                        floating /= 10;
                        ++expo;
                    }
 
                    int form = lengthSpec - precSpec - expo - (precSpec || altForm ? 1 : 0);
                    if (emode) {
                        form -= 4;      // 'e+00'
                    }
                    if (form < 0) {
                        form = 0;
                    }
 
                    __int_str(floating, intStrBuffer, base, plusSign, spaceNoSign, form, \
                              leftJustify, zeroPad);
 
                    displayString(intStrBuffer, &chars, write);
 
                    floating -= (int) floating;
 
                    for (int i = 0; i < precSpec; ++i) {
                        floating *= 10;
                    }
                    long decPlaces = (long) (floating + 0.5);
 
                    if (precSpec) {
                        displayCharacter('.', &chars, write);
                        __int_str(decPlaces, intStrBuffer, 10, false, false, 0, false, false);
                        intStrBuffer[precSpec] = 0;
                        displayString(intStrBuffer, &chars, write);
                    } else if (altForm) {
                        displayCharacter('.', &chars, write);
                    }
 
                    break;
                }
 
 
                case 'a':
                case 'A':
                    //ACK! Hexadecimal floating points...
                    break;
 
                default:
                    break;
            }
 
            if (specifier == 'e') {
                displayString("e+", &chars, write);
            } else if (specifier == 'E') {
                displayString("E+", &chars, write);
            }
 
            if (specifier == 'e' || specifier == 'E') {
                __int_str(expo, intStrBuffer, 10, false, false, 2, false, true);
                displayString(intStrBuffer, &chars, write);
            }
 
        } else {
            displayCharacter(format[i], &chars, write);
        }
    }
 
    return chars;
}
 
/** printf:
 *  Prints a formatted string to the screen
 *  
 *  @param format The formatted string to print
 *  @returns On success returns the number of characters written, else a negetive number is returned
 */
__attribute__ ((format (printf, 1, 2))) int printf (const char* format, ...)
{
    va_list list;
    va_start (list, format);
    int i = vprintf (format, list, fb_write);
    va_end (list);
    return i;
 
}

/** printf:
 *  Sends a formatted string to a driver
 *  
 *  @param format The formatted string to send
 *  @param write The driver write function to use
 *  @returns On success returns the number of characters written, else a negetive number is returned
 */
__attribute__ ((format (printf, 1, 3))) int printfw (const char* format, write_func write, ...)
{
    va_list list;
    va_start (list, write);
    int i = vprintf (format, list, write);
    va_end (list);
    return i;
 
}