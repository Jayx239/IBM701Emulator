#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define NUM_OPCODES 24
#include "program_counter.h"
#include "bitmath.h"

struct opcode {
    int value;
    char* key;
    int key_size;
};

void generate_opcodes(struct opcode opcodes[]);
void create_instruction(struct program_counter *pc,int* Machine_Memory, struct opcode command_opcode, int address);
void display_opcodes();
void print_binary(int value);
struct opcode get_opcode(char* code, struct opcode opcodes[]);


#endif
