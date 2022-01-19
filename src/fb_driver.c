#include "io.h"

// The I/O ports
#define FB_COMMAND_PORT 0X3D4
#define FB_DATA_PORT    0x3D5

// The I/O port commands
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

//Framebuffer data
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/*The framebuffer pointer*/
unsigned char * const fb = (unsigned char*) 0xb8000;

/*Definition for framebuffer colors*/
#define FB_BLACK        0X0
#define FB_BLUE         0X1
#define FB_GREEN        0X2
#define FB_CYAN         0X3
#define FB_RED          0X4
#define FB_MAGENTA      0X5
#define FB_BROWN        0X6
#define FB_LIGHT_GREY   0X7
#define FB_DARK_GREY    0X8
#define FB_LIGHT_BLUE   0X9
#define FB_LIGHT_GREEN  0XA
#define FB_LIGHT_CYAN   0XB
#define FB_LIGHT_RED    0XC
#define FB_LIGHT_MAGENTA 0XD
#define FB_LIGHT_BROW   0XE
#define FB_WHITE        0XF

/** fb_move_cusror
 * Moves the cursor of the framebuffer to the given position
 * 
 * @param pos The new position of the cursor
*/
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos  & 0x00FF);
}
/** fb_get_cursor_position
 * Gets the framebuffer cursor position
 * 
 * @returns The cursor position of the framebuffer
*/
unsigned short fb_get_cursor_position(void)
{
    unsigned short pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((unsigned short)inb(0x3D5)) << 8;
    return pos;
}

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

/** fb_handle_scrolling
 * Handles scrolling down the frambuffer
*/
void fb_handle_scrolling(void)
{
    //Move the characters up
    for(int i = 0; i < VGA_HEIGHT - 1; ++i)
    {
        for(int j = 0; j < VGA_WIDTH; ++j)
        {
            ((short*)fb)[i * VGA_WIDTH + j] = ((short*)fb)[(i + 1) * VGA_WIDTH + j];
        }
    }

    //Fill the last row with spaces
    for(int i = 0; i < VGA_WIDTH; ++i)
    {
        fb_write_cell((VGA_WIDTH * (VGA_HEIGHT - 1) + i) * 2, ' ', FB_WHITE, FB_BLACK);
    }
}


/** fb_write
 * Writes a buffer of length len to the screen.
 * 
 * @param buf The buffer being written to the screen
 * @param len The length of the buffer being written
 * @returns The number of characters written to the screen, returns -1 if failed
*/
int fb_write(const char* buf, unsigned int len)
{
    if(buf == 0)
        return -1;
    unsigned short cursor_pos = fb_get_cursor_position();
    
    for(unsigned int i = 0; i < len; ++i)
    {
        if(buf[i] == ' ')
            cursor_pos += 1;
        else if(buf[i] == '\t')
            cursor_pos = (cursor_pos + 4) - (cursor_pos % 4);
        else if(buf[i] == '\n')
            cursor_pos = (cursor_pos + VGA_WIDTH) - (cursor_pos % VGA_WIDTH);
        else if(buf[i] == '\b');
        else
        {
            fb_write_cell(cursor_pos * 2, buf[i], FB_WHITE, FB_BLACK);
            cursor_pos += 1;
        }

        if(cursor_pos >= VGA_WIDTH * VGA_HEIGHT)
        {
            fb_handle_scrolling();
            cursor_pos -= VGA_WIDTH;
        }
    }

    fb_move_cursor(cursor_pos);

    return len;
}
