#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

// Interrupt vectors
#define VEC_PRIVILEGE   0x00
#define VEC_ILLEGAL_OP  0x01
#define VEC_KEYBOARD    0x80
#define VEC_TIMER       0x81

// Processor status register (PSR) bit definitions
#define PSR_PRIV  (1 << 15)
#define PSR_N     (1 << 2)
#define PSR_Z     (1 << 1)
#define PSR_P     (1 << 0)

// Stack pointers
extern uint16_t super_sp;
extern uint16_t user_sp;

// Interrupt state
extern int interrupt_enabled;

// Interrupt functions
void init_interrupts(void);
void trigger_interrupt(uint8_t vector);
void check_interrupts(void);
void enable_interrupts(void);
void disable_interrupts(void);

#endif /* INTERRUPT_H */
