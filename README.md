# LC-3 Virtual Machine

A simple implementation of the LC-3 (Little Computer 3) virtual machine in C. This VM can execute LC-3 machine code programs.

## Overview

The LC-3 is a reduced instruction set computer (RISC) architecture designed for educational purposes. This implementation provides a complete virtual machine that can load and execute LC-3 binary programs.

## Features

- Full LC-3 instruction set implementation
- 65,536 memory locations (16-bit addressable)
- 8 general-purpose registers (R0-R7)
- Program counter (PC) and condition flags
- Memory-mapped keyboard I/O
- TRAP routines for I/O operations
- Proper signal handling (Ctrl+C)

## Project Structure

```
vm/
├── Makefile
├── README.md
├── include/
│   ├── architecture.h           # Core definitions and constants
│   ├── memory.h        # Memory operations
│   ├── terminal.h      # Terminal I/O control
│   └── trap.h          # TRAP instruction handlers
└── src/
    ├── main.c          # Main program loop
    ├── architecture.c  # Core VM utilities
    ├── memory.c        # Memory read/write and image loading
    ├── terminal.c      # Terminal buffering control
    └── trap.c          # TRAP instruction implementations
```

## Building

### Prerequisites
- GCC compiler
- Make
- POSIX-compliant system (Linux, macOS, WSL)

### Compile
```bash
make
```

### Clean
```bash
make clean
```

## Usage

```bash
./vm <image-file> [additional-image-files...]
```

### Example
```bash
./vm program.obj
```

You can load multiple image files:
```bash
./vm program1.obj program2.obj
```

## Supported Instructions

### Arithmetic & Logic
- `ADD` - Addition
- `AND` - Bitwise AND
- `NOT` - Bitwise NOT

### Memory Operations
- `LD` - Load (PC-relative)
- `LDI` - Load Indirect
- `LDR` - Load Base+Offset
- `LEA` - Load Effective Address
- `ST` - Store (PC-relative)
- `STI` - Store Indirect
- `STR` - Store Base+Offset

### Control Flow
- `BR` - Conditional Branch
- `JMP` - Jump (includes RET)
- `JSR/JSRR` - Jump to Subroutine

### TRAP Routines
- `GETC` (0x20) - Read a character from keyboard
- `OUT` (0x21) - Output a character
- `PUTS` (0x22) - Output a string
- `IN` (0x23) - Prompt for and read a character
- `PUTSP` (0x24) - Output a byte string
- `HALT` (0x25) - Halt the program

## Memory Map

- `0x0000 - 0x2FFF` - System and interrupt vectors
- `0x3000 - 0xFDFF` - User program space (default start: 0x3000)
- `0xFE00 - 0xFE01` - Memory-mapped I/O (keyboard)
- `0xFE02 - 0xFFFF` - Reserved

## Implementation Details

### Registers
- **R0-R7**: General-purpose registers
- **PC**: Program Counter
- **COND**: Condition flags (Positive, Zero, Negative)

### Condition Flags
- `FL_POS` - Positive result
- `FL_ZRO` - Zero result
- `FL_NEG` - Negative result

### Memory-Mapped Registers
- `KBSR (0xFE00)` - Keyboard Status Register
- `KBDR (0xFE02)` - Keyboard Data Register

## Notes

- Programs must be in LC-3 binary format (.obj files)
- The VM starts execution at address 0x3000 by default
- Press Ctrl+C to terminate execution
- Terminal buffering is disabled during execution for immediate keyboard input

## Example Programs

To test the VM, you can use sample LC-3 programs:
- Hello World
- 2048 Game
- Rogue (dungeon crawler)

## License

This is an educational project. Feel free to use and modify as needed.
