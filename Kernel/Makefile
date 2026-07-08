# Makefile
ASM=nasm
CC=gcc
LD=ld

ASMFLAGS=-f elf64
CFLAGS=-ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -m64 -c
LDFLAGS=-n -T linker.ld

OBJECTS=multiboot_header.o \
        boot.o \
        long_mode_init.o \
        kernel.o

kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

iso: kernel.bin
	@rm -rf isodir
	@mkdir -p isodir/boot/grub
	@cp kernel.bin isodir/boot/
	@cp grub.cfg isodir/boot/grub/
	@grub-mkrescue -o kernel.iso isodir 2>/dev/null || (echo "Error: grub-mkrescue failed. Installing xorriso..." && sudo apt-get install xorriso -y && grub-mkrescue -o kernel.iso isodir)

run: iso
	qemu-system-x86_64 -cdrom kernel.iso -monitor stdio

clean:
	@rm -f *.o kernel.bin kernel.iso
	@rm -rf isodir