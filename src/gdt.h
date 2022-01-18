#include "stddef.h"

#define GDT_ENTRY_SIZE 8

typedef struct 
{
    unsigned long base;
    unsigned long limit;
    unsigned char type;
} GDT_entry;

/** encode_gdt_entry:
 *  Encodes the tidy GDT structure into the messy x86 structure
 * 
 *  @param des The destination pointer for the structure
 *  @param src The source GDT entry that is encoded
 *  @returns 1 if Succesfull, else 0
 */
int encode_gdt_entry(unsigned char* des, GDT_entry src);

/** create_gdt:
 *  Creates a gdt in a specified buffer. (note, the buffer must be of sufficient size)
 * 
 *  @param des The pointer to the buffer to move the GDT to.
 *  @param src The list of GDT entries.
 *  @param len The Amount of GDT entries.
 */
void create_gdt(unsigned char* des, const GDT_entry* src, unsigned int len);
/** load_gdt:
 *  Loads a gdt buffer to the gdt table, and resets segment registers
 *  (code to the first entry and data to the second)
 * 
 *  @param buf The buffer pointing to the gdt entries
 *  @param len The length of the buffer
 */
void load_gdt(unsigned char* buf, unsigned short len);