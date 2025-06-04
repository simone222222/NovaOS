# Makefile for NovaOS

# Cross-compiler tools
AS = nasm
CC = i686-elf-gcc
LD = i686-elf-ld

# Build flags
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ikernel \
         -fno-stack-protector -m32 -march=i686 -nostdlib -nostartfiles -nodefaultlibs
LDFLAGS = -T linker.ld -O2 -nostdlib -m elf_i386
ASFLAGS = -f elf32

# Kernel source files (C)
KERNEL_C_SOURCES = $(wildcard kernel/*.c)
KERNEL_C_OBJECTS = $(patsubst kernel/%.c, build/kernel/%.o, $(KERNEL_C_SOURCES))

# Boot source file (Assembly)
BOOT_ASM_SOURCE = boot/boot.asm
BOOT_ASM_OBJECT = build/boot/boot.o

# Output binary
KERNEL_BIN = build/novaos.bin
ISO_FILE = novaos.iso

# All target: build the ISO
all: $(ISO_FILE)

# Link the kernel
$(KERNEL_BIN): $(BOOT_ASM_OBJECT) $(KERNEL_C_OBJECTS)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "LD   $@"

# Compile C kernel files
build/kernel/%.o: kernel/%.c kernel/*.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC   $<"

# Assemble boot file
$(BOOT_ASM_OBJECT): $(BOOT_ASM_SOURCE)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@
	@echo "AS   $<"

# Create the ISO
$(ISO_FILE): $(KERNEL_BIN) grub/grub.cfg
	@mkdir -p iso/boot/grub
	cp $(KERNEL_BIN) iso/boot/novaos.bin
	cp grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_FILE) iso
	@echo "ISO  $@"

# Run with QEMU
run: $(ISO_FILE)
	qemu-system-i386 -cdrom $(ISO_FILE) -m 128M

# Clean build files
clean:
	rm -rf build iso $(ISO_FILE)
	@echo "Cleaned."

.PHONY: all clean run