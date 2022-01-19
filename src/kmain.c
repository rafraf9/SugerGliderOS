#include "print_string.h"
#include "logging.h"
#include "gdt.h"
#include "idt.h"
#include "io.h"
#include "keyboard_driver.h"
#include "multiboot.h"
#include "breakpoint.h"

static GDT_entry gdt_entries[2];
typedef void (*call_module)(void);

void kmain(unsigned int ebx)
{
    multiboot_info_t* mbinfo = (multiboot_info_t*)ebx;      //Get the mutliboot header from GRUB
    if(mbinfo->flags | 0x4 && mbinfo->mods_count != 1)                             //Check that the module was loaded
    {
        printf("Module failed to load!");
        log_error("Module failed to load");
        while(1);
    }

    unsigned int address_of_module = mbinfo->mods_addr; //Get the module address


    unsigned char buffer[GDT_ENTRY_SIZE * 3];

    gdt_entries[0] = (GDT_entry){.base=0x0, .limit=0xffffffff, .type=0x9A}; //The code segment.
    gdt_entries[1] = (GDT_entry){.base=0x0, .limit=0xffffffff, .type=0x92}; //The data segment.

    log_info("Creating GDT...");
    create_gdt(buffer, gdt_entries, 2);
    log_info("Loading GDT..."); 

    load_gdt(buffer, sizeof(buffer));
    printf("Succesfully loaded new GDT\n");
    log_info("Succesfully loaded GDT");

    log_info("Loading IDT..."); 
    idt_init();

    printf("Succesfully loaded new IDT\n");
    log_info("Succesfully loaded IDT");

    call_module start_program = *((call_module*)address_of_module);
    start_program();

    init_keyboard();

    sti();

    while(1);
}