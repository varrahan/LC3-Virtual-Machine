#include "architecture.h"
#include "interrupt.h"
#include "memory.h"

uint16_t super_sp = 0x3000;
uint16_t user_sp = 0xFE00;

int interrupt_enabled = 0;
static int pending_interrupt = 0;
static uint8_t pending_vector = 0;

void init_interrupts(void) {
    interrupt_enabled = 1;
    pending_interrupt = 0;
    pending_vector = 0;
    super_sp = 0x3000;
    user_sp = 0xFE00;
}

void trigger_interrupt(uint8_t vector) {
    pending_interrupt = 1;
    pending_vector = vector;
}

void check_interrupts(void) {
    if (!interrupt_enabled || !pending_interrupt) {
        return;
    }

    if (reg[R_PSR] & PSR_PRIV) {
        user_sp = reg[R_R6];
        reg[R_R6] = super_sp;
    }
    reg[R_R6]--;
    mem_write(reg[R_R6], reg[R_PSR]);

    reg[R_R6]--;
    mem_write(reg[R_R6], reg[R_PC]);

    reg[R_PSR] &= ~PSR_PRIV;
    uint16_t vector_addr = 0x0100 + pending_vector;
    reg[R_PC] = mem_read(vector_addr);
    pending_interrupt = 0;
}

void enable_interrupts(void) {
    interrupt_enabled = 1;
}

void disable_interrupts(void) {
    interrupt_enabled = 0;
}