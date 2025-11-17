#include "architecture.h"

// Global memory and registers
uint16_t memory[MEMORY_MAX];
uint16_t reg[R_COUNT];

// Sign extend a value from bit_count bits to 16 bits
uint16_t sign_extend(uint16_t x, int bit_count) {
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

// Update condition flags based on register value
void update_flags(uint16_t r) {
    if (reg[r] == 0) {
        reg[R_COND] = FL_ZRO;
        reg[R_PSR] = (reg[R_PSR] & ~0x7) | FL_ZRO;
    } else if (reg[r] >> 15) {
        reg[R_COND] = FL_NEG;
        reg[R_PSR] = (reg[R_PSR] & ~0x7) | FL_NEG;
    } else {
        reg[R_COND] = FL_POS;
        reg[R_PSR] = (reg[R_PSR] & ~0x7) | FL_POS;
    }
}

// Swap endianness of 16-bit value
uint16_t swap16(uint16_t x) {
    return (x << 8) | (x >> 8);
}