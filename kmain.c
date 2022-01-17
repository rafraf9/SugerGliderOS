#include "fb_driver.h"

void kmain(void)
{
    const char* msg = "Hello World!\n wow this works ";
    unsigned int msg_len = 28;

    for(int i = 0; i < 80; ++i)
        fb_write(msg, msg_len);

    while(1);
}