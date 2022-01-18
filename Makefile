OBJECTS = loader.o kmain.o io.o fb_driver.o serial_driver.o print_string.o string.o logging.o \
        gdt.o load_gdt.o
PRE_OBJECTS = $(addprefix bin/,$(OBJECTS))
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
        -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(PRE_OBJECTS)
	ld $(LDFLAGS) $(PRE_OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
                    -b boot/grub/stage2_eltorito    \
                    -no-emul-boot                   \
                    -boot-load-size 4               \
                    -A os                           \
                    -input-charset utf8             \
                    -quiet                          \
                    -boot-info-table                \
                    -o os.iso                       \
                    iso
run: os.iso
	bochs -f bochsrc.txt -q

bin/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@
bin/%.o: src/%.asm
	$(AS) $(ASFLAGS) $< -o $@
clean:
	rm -rf bin/*.o kernel.elf os.iso