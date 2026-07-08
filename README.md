# OS-from-scratch

A basic, educational Operating System developed from scratch. This project serves as an exploration into the fundamentals of operating system design and low-level programming.

## Overview

This repository contains the source code for a minimalistic operating system. It demonstrates core OS concepts such as bootloading, kernel initialization, and basic hardware interaction. It is designed to be a learning resource for understanding how hardware and software bridge together at the most fundamental level.

## Features (Current & Planned)

- **Bootloader**: Custom boot sector code to transition from BIOS.
- **Basic Kernel**: A rudimentary kernel written in C/Assembly.
- **VGA Text Mode**: Basic screen output and character rendering.
- **Interrupt Handling** (Planned/WIP): Setting up the IDT (Interrupt Descriptor Table).
- **Keyboard Input** (Planned/WIP): Basic PS/2 keyboard driver.

## Prerequisites

To build and run this OS, you will need the following tools installed on your system:

- **NASM**: The Netwide Assembler, for compiling assembly code.
- **GCC**: Cross-compiler for generating the kernel executable (e.g., `i686-elf-gcc`).
- **QEMU**: A generic and open source machine emulator and virtualizer (for testing without real hardware).
- **Make**: For automating the build process.

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/[Repository-Name].git
cd [Repository-Name]
```

### 2. Build the OS

You can use the provided `Makefile` to compile the source code and build the OS image:

```bash
make all
```

### 3. Run in Emulator

To run the compiled OS image using QEMU:

```bash
make run
```
*(Alternatively, you can run QEMU manually: `qemu-system-i386 -fda os-image.bin`)*

## Project Structure

- `boot/`: Contains the assembly code for the bootloader.
- `kernel/`: Contains the core C and assembly files for the kernel.
- `drivers/`: Basic hardware drivers (e.g., screen, keyboard).
- `Makefile`: Build instructions for the project.

## License

This project is open-source and available under the [MIT License](LICENSE).
