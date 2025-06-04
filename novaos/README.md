# NovaOS

A simple operating system written in C and assembly for educational purposes.

## Overview

NovaOS is a basic operating system that demonstrates fundamental OS concepts including:
- Boot process
- Kernel initialization
- Basic shell interface
- VGA text mode output
- Keyboard input handling

## Building

### Prerequisites

- i686-elf cross-compiler toolchain
- NASM (Netwide Assembler)
- Make
- QEMU (for testing)

### Build Steps

1. Ensure you have the cross-compiler toolchain installed
2. Run `make` to build the kernel
3. Run `make run` to test in QEMU

## Project Structure

```
novaos/
├── boot/           # Bootloader and early initialization
├── kernel/         # Kernel source code
├── build/          # Build output directory
└── grub/           # GRUB configuration
```

## Features

- Basic shell with commands:
  - help: Show available commands
  - echo: Echo input text
  - clear: Clear the screen
  - reboot: Reboot the system
  - color: Test VGA colors

## License

This project is open source and available under the MIT License. 