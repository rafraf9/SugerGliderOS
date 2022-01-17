#include "fb_driver.h"
#include "serial_driver.h"

void kmain(void)
{
    const char* msg = "Hello World!\n wow this works ";
    unsigned int msg_len = 28;

    for(int i = 0; i < 20; ++i)
        serial_write(msg, msg_len);

    while(1);
}