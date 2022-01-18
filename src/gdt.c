#include "gdt.h"
#include "string.h"

typedef struct
{
    unsigned int address;
    unsigned short size;
} __attribute__((packed)) GDTR;

/** load_gdt_asm:
 *  Calls assembly code to load the gdt
 * 
 *  @param gdtr pointer to gdt descriptor
 */
void load_gdt_asm(GDTR gdtr);

/** encode_gdt_entry:
 *  Encodes the tidy Gdt structure into the messy x86 structure
 * 
 *  @param des The destination pointer for the structure
 *  @param src The source gdt entry that is encoded
 *  @returns 1 if Succesfull, else 0
 */
int encode_gdt_entry(unsigned char *des, GDT_entry src)
{
    //Credit to OS DEV wiki for the code
    // Check the limit to make sure that it can be encoded
    if ((src.limit > 65536) && ((src.limit & 0xFFF) != 0xFFF))
    {
        return 0;
    }
    if (src.limit > 65536)
    {
        // Adjust granularity if required
        src.limit = src.limit >> 12;
        des[6] = 0xC0;
    }
    else
    {
        des[6] = 0x40;
    }

    // Encode the limit
    des[0] = src.limit & 0xFF;
    des[1] = (src.limit >> 8) & 0xFF;
    des[6] |= (src.limit >> 16) & 0xF;

    // Encode the base
    des[2] = src.base & 0xFF;
    des[3] = (src.base >> 8) & 0xFF;
    des[4] = (src.base >> 16) & 0xFF;
    des[7] = (src.base >> 24) & 0xFF;

    // And... Type
    des[5] = src.type;
    return 1;
}

/** create_gdt:
 *  Creates a gdt in a specified buffer. (note, the buffer must be of sufficient size)
 * 
 *  @param des The pointer to the buffer to move the GDT to.
 *  @param src The list of GDT entries.
 *  @param len The Amount of GDT entries.
 */
void create_gdt(unsigned char *des, const GDT_entry *src, unsigned int len)
{
    //Fill the null entry
    memset(des, '\0', 8);

    //Copy the rest of the entries
    for (unsigned int i = 0; i < len; ++i)
    {
        encode_gdt_entry(des + (i + 1) * 8, src[i]);
    }
}

/** load_gdt:
 *  Loads a gdt buffer to the gdt table, and resets segment registers
 *  (code to the first entry and data to the second)
 * 
 *  @param buf The buffer pointing to the gdt entries
 *  @param len The length of the buffer
 */
void load_gdt(unsigned char *buf, unsigned short len)
{
    GDTR gdt_reg = {.address = (unsigned int)buf, .size = len - 1};
    load_gdt_asm(gdt_reg);
}