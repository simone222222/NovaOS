; boot.asm
BITS 32                 ; We are in 32-bit protected mode

; Multiboot header constants
MB_ALIGN            EQU 1<<0    ; Align loaded modules on page boundaries
MB_MEMINFO          EQU 1<<1    ; Provide memory map
MB_MAGIC            EQU 0x1BADB002 ; Multiboot magic number
MB_FLAGS            EQU MB_ALIGN | MB_MEMINFO
MB_CHECKSUM         EQU -(MB_MAGIC + MB_FLAGS)

section .multiboot
align 4
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB stack
stack_top:

section .text
global _start
extern kmain           ; kmain is defined in C

_start:
    ; Set up the stack
    mov esp, stack_top

    ; Push multiboot info pointer and magic number (GRUB compliance)
    push eax ; Multiboot magic number (already in eax from bootloader)
    push ebx ; Multiboot info structure pointer

    ; Call the C kernel main function
    call kmain

    ; Halt the CPU if kmain returns (it shouldn't)
    cli
.hang:
    hlt
    jmp .hang