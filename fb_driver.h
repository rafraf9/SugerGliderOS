#ifndef FB_DRIVER_H
#define FB_DRIVER_H

/** fb_write
 * Writes a buffer of length len to the screen.
 * 
 * @param buf The buffer being written to the screen
 * @param len The length of the buffer being written
 * @returns The number of characters written to the screen, returns -1 if failed
*/
int fb_write(const char* buf, unsigned int len);

#endif /*FB_DRIVER_H*/