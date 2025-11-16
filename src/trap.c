#include "trap.h"
#include "architecture.h"
#include <stdio.h>

// Handle TRAP instructions - returns 0 to halt, 1 to continue
int handle_trap(uint16_t instr) {
    reg[R_R7] = reg[R_PC];
    
    switch (instr & 0xFF) {
        case TRAP_GETC: {
            reg[R_R0] = (uint16_t)getchar();
            update_flags(R_R0);
            break;
        }
        case TRAP_OUT: {
            putc((char)reg[R_R0], stdout);
            fflush(stdout);
            break;
        }
        case TRAP_PUTS: {
            uint16_t* c = memory + reg[R_R0];
            while (*c) {
                putc((char)*c, stdout);
                ++c;
            }
            fflush(stdout);
            break;
        }
        case TRAP_IN: {
            printf("Enter a character: ");
            char c = getchar();
            putc(c, stdout);
            fflush(stdout);
            reg[R_R0] = (uint16_t)c;
            update_flags(R_R0);
            break;
        }
        case TRAP_PUTSP: {
            uint16_t* c = memory + reg[R_R0];
            while (*c) {
                char char1 = (*c) & 0xFF;
                putc(char1, stdout);
                char char2 = (*c) >> 8;
                if (char2) putc(char2, stdout);
                ++c;
            }
            fflush(stdout);
            break;
        }
        case TRAP_HALT: {
            puts("HALT");
            fflush(stdout);
            return 0;
        }
    }
    return 1;
}
