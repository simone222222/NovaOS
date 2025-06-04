/* linker.ld */
ENTRY(_start) /* The entry point is defined in boot.asm */
OUTPUT_FORMAT(elf32-i386) /* Output format for 32-bit x86 */

SECTIONS {
    /* Kernel starts at 1MB */
    . = 1M; /* Physical address 0x00100000 */

    .text : {
        *(.multiboot) /* Ensure multiboot header is first */
        *(.text*)     /* All text sections */
        *(.rodata*)   /* All read-only data sections */
    }

    .data : {
        *(.data*)     /* All initialized data sections */
    }

    .bss : {
        *(.bss*)      /* All uninitialized data sections */
        *(COMMON)     /* Common symbols */
    }

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}