#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "architecture.h"
#include "interrupt.h"
#include "memory.h"
#include "terminal.h"
#include "trap.h"

void handle_interrupt(int signal) {
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        printf("At least 2 arguements are required: [image-file1] ...\n");
        exit(2);
    }

    for (int j = 1; j < argc; ++j) {
        if (!read_image(argv[j])) {
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
        }
    }
    
    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    init_interrupts();
    
    reg[R_COND] = FL_ZRO;
    reg[R_PSR] = PSR_PRIV | FL_ZRO;

    enum { PC_START = START_ADDR_DEFAULT };
    reg[R_PC] = PC_START;

    int running = 1;
    while (running) {

        check_interrupts();
        
        uint16_t instr = mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12;

        switch (op) {
            case OP_ADD: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t src_reg1 = (instr >> 6) & 0x7;
                uint16_t imm_flag = (instr >> 5) & 0x1;

                if(imm_flag) {
                    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
                    reg[dest_reg] = reg[src_reg1] + imm5;
                } else {
                    uint16_t src_reg2 = instr & 0x7;
                    reg[dest_reg] = reg[src_reg1] + reg[src_reg2];
                }

                update_flags(dest_reg);
                break;
            }
            case OP_AND: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t src_reg1 = (instr >> 6) & 0x7;
                uint16_t imm_flag = (instr >> 5) & 0x1;

                if (imm_flag) {
                    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
                    reg[dest_reg] = reg[src_reg1] & imm5;
                } else {
                    uint16_t src_reg2 = instr & 0x7;
                    reg[dest_reg] = reg[src_reg1] & reg[src_reg2];
                }

                update_flags(dest_reg);
                break;
            }
            case OP_NOT: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t src_reg1 = (instr >> 6) & 0x7;
                reg[dest_reg] = ~reg[src_reg1];
                update_flags(dest_reg);
                break;
            }
            case OP_BR: {
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                uint16_t cond_flag = (instr >> 9) & 0x7;
                if (cond_flag & reg[R_COND]) {
                    reg[R_PC] += pc_offset;
                }
                break;
            }
            case OP_JMP: {
                uint16_t src_reg1 = (instr >> 6) & 0x7;
                reg[R_PC] = reg[src_reg1];
                break;
            }
            case OP_JSR: {
                uint16_t long_flag = (instr >> 11) & 1;
                reg[R_R7] = reg[R_PC];
                if (long_flag) {
                    uint16_t long_pc_offset = sign_extend(instr & 0x7FF, 11);
                    reg[R_PC] += long_pc_offset;
                } else {
                    uint16_t src_reg1 = (instr >> 6) & 0x7;
                    reg[R_PC] = reg[src_reg1];
                }
                break;
            }
            case OP_LD: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                reg[dest_reg] = mem_read(reg[R_PC] + pc_offset);
                update_flags(dest_reg);
                break;
            }
            case OP_LDI: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                reg[dest_reg] = mem_read(mem_read(reg[R_PC] + pc_offset));
                update_flags(dest_reg);
                break;
            }
            case OP_LDR: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t src_reg1 = (instr >> 6) & 0x7;
                uint16_t offset = sign_extend(instr & 0x3F, 6);
                reg[dest_reg] = mem_read(reg[src_reg1] + offset);
                update_flags(dest_reg);
                break;
            }
            case OP_LEA: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                reg[dest_reg] = reg[R_PC] + pc_offset;
                update_flags(dest_reg);
                break;
            }
            case OP_ST: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                mem_write(reg[R_PC] + pc_offset, reg[dest_reg]);
                break;
            }
            case OP_STI: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                mem_write(mem_read(reg[R_PC] + pc_offset), reg[dest_reg]);
                break;
            }
            case OP_STR: {
                uint16_t dest_reg = (instr >> 9) & 0x7;
                uint16_t src_reg1 = (instr >> 6) & 0x7;
                uint16_t offset = sign_extend(instr & 0x3F, 6);
                mem_write(reg[src_reg1] + offset, reg[dest_reg]);
                break;
            }
            case OP_TRAP: {
                running = handle_trap(instr);
                break;
            }
            case OP_RTI: {
                if (reg[R_PSR] & PSR_PRIV) {
                    trigger_interrupt(VEC_PRIVILEGE);
                } else {
                    reg[R_PC] = mem_read(reg[R_R6]);
                    reg[R_R6]++;
                    
                    reg[R_PSR] = mem_read(reg[R_R6]);
                    reg[R_R6]++;
                    reg[R_COND] = reg[R_PSR] & 0x7;
                
                    if (reg[R_PSR] & PSR_PRIV) {
                        super_sp = reg[R_R6];
                        reg[R_R6] = user_sp;
                    }
                }
                break;
            }
            case OP_RES: {
                trigger_interrupt(VEC_ILLEGAL_OP);
                break;
            }
            default:
                trigger_interrupt(VEC_ILLEGAL_OP);
                break;
        }
    }
    restore_input_buffering();
    return 0;
}