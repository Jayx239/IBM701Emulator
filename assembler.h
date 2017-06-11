#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define NUM_OPCODES 24

struct opcode {
    int value;
    char* key;
    int key_size;
};

void generate_opcodes(struct opcode opcodes[]);
void display_opcodes();
void print_binary(int value);
struct opcode get_opcode(char* code, struct opcode opcodes[]);

#endif
