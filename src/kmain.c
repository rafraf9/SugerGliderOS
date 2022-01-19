#include "print_string.h"
#include "logging.h"
#include "gdt.h"
#include "breakpoint.h"
#include "idt.h"
#include "io.h"
#include "keyboard_driver.h"

static GDT_entry gdt_entries[2];

void kmain(void)
{
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

    init_keyboard();

    sti();

    while(1);
}