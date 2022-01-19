#include "keyboard_driver.h"
#include "idt.h"
#include "io.h"
#include "print_string.h"
#include "logging.h"

#define KBD_IRQ_NUM 0x1
#define KBD_DATA_PORT 0x60

static unsigned char scan_code_to_ascii[256] = {
    0x0, 0x0,               //ERROR, ESCAPE
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '+', 0x0, //BACKSPACE
    '\t', //TAB
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', //ENTER
    0x0, //Left Control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
    0x0, //Left Shift
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',
    0x0, //Right shift
    '*', //Numpad
    0x0, //Left Alt
    ' ',
    0x0, //Caps Lock
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,   //F1 - F10
    0x0, 0x0,        //Numlock and Scroll lock
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', //NumPad
    0x0 //The rest is zeros
};

//The keyboard handler: for now just prints the ascii code to the screen
void keyboard_driver(__attribute__((unused)) unsigned char irq)
{
    unsigned char scancode = inb(KBD_DATA_PORT);
    //Check if its an error
    if(scancode == 0x0)
    {
        printf("Keyboard Error!\n");
        log_error("Keyboard Failed");
    }

    unsigned char c = scan_code_to_ascii[scancode]; //Get the ascii character
    if(c != 0)
        putchar(c);
}

/** init_keyboard:
 *  Initialize the keyboard driver/
 */
void init_keyboard(void)
{
    register_irq_handler(KBD_IRQ_NUM, keyboard_driver); //Register the keyboard handler
}