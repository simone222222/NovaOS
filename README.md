![image](https://github.com/user-attachments/assets/c3f76733-5069-4702-bca7-9fc8c8b3fa65)

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

# Known Issues
if on virtualbox and typing reboot it will cause a guru meditation
