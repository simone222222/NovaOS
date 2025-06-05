![image](https://github.com/user-attachments/assets/7a71b5fb-250c-4e85-be7a-5e29d64a5f13)


# NovaOS
the 2nd working os in c

# How do i compile?

you must have:
i686-elf toolchain
  
  Tutorial from here:https://wiki.osdev.org/GCC_Cross-Compiler

nasm

mtools

xorriso

grub

make

qemu


install with:
    sudo apt update
    
    sudo apt-get install make nasm grub-pc-bin mtools xorriso qemu qemu-system

and run `make`

# Testing
run `make run` to test in qemu
