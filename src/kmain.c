#include "print_string.h"
#include "gdt.h"

static GDT_entry gdt_entries[2];

void kmain(void)
{
    unsigned char buffer[GDT_ENTRY_SIZE * 3];

    gdt_entries[0] = (GDT_entry){.base=0x0, .limit=0xffffffff, .type=0x9A}; //The code segment.
    gdt_entries[1] = (GDT_entry){.base=0x0, .limit=0xffffffff, .type=0x92}; //The data segment.

    create_gdt(buffer, gdt_entries, 2);
    
    for(unsigned int i = 0; i < 24; ++i)
    {
        printf("%x", buffer[i] & 0xff);
    }

    load_gdt(buffer, sizeof(buffer));
    printf("\nSuccesfully loaded new GDT");

    while(1);
}