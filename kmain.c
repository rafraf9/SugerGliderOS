#include "logging.h"

void kmain(void)
{
    log_debug("%s", "Debug message");
    log_info("%s", "Info message");
    log_error("%s", "Error message");


    while(1);
}