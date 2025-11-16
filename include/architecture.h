#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <stdint.h>

// Memory configuration
#define MEMORY_MAX (1 << 16)
#define START_ADDR_DEFAULT 0x3000

// General purpose registers
enum {
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC, 
    R_COND,
    R_COUNT
};

// Memory mapped registers
enum {
    MR_KBSR = 0xFE00,
    MR_KBDR = 0xFE02
};

// Op codes for instruction set
enum {
    OP_BR = 0,
    OP_ADD,
    OP_LD,
    OP_ST,
    OP_JSR,
    OP_AND,
    OP_LDR,
    OP_STR,
    OP_RTI,
    OP_NOT,
    OP_LDI,
    OP_STI,
    OP_JMP,
    OP_RES,
    OP_LEA,
    OP_TRAP
};

// ALU condition flags
enum {
    FL_POS = 1 << 0,
    FL_ZRO = 1 << 1,
    FL_NEG = 1 << 2
};

// Trap codes
enum { 
    TRAP_GETC = 0x20,
    TRAP_OUT = 0x21,
    TRAP_PUTS = 0x22,
    TRAP_IN = 0x23,
    TRAP_PUTSP = 0x24,
    TRAP_HALT = 0x25
};

// Global memory and registers
extern uint16_t memory[MEMORY_MAX];
extern uint16_t reg[R_COUNT];

// Utility functions
uint16_t sign_extend(uint16_t x, int bit_count);
void update_flags(uint16_t r);
uint16_t swap16(uint16_t x);

#endif /* ARCHITECTURE_H */