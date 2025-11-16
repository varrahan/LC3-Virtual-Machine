#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <sys/termios.h>

// Terminal control
extern struct termios original_tio;

void disable_input_buffering(void);
void restore_input_buffering(void);
uint16_t check_key(void);

#endif /* TERMINAL_H */