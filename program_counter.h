#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

#define INSTRUCTION_SIZE 18 
#define MULTIPLIER_QUOTIENT_SIZE 36
#define ACCUMULATOR_SIZE 38
#define OPCODE_SIZE 6
#define WORD_SIZE 36
#define MEMORY_SIZE 2048
#define ADDRESS_SIZE 12

struct program_counter {
    
    int current_address;
    int offset; // 0 or 1
    int address_word[WORD_SIZE];
    int instruction[INSTRUCTION_SIZE];
    int multiplier_quotient[MULTIPLIER_QUOTIENT_SIZE];
    int accumulator[ACCUMULATOR_SIZE];
};

void increment_counter(struct program_counter *pc);
void decrement_counter(struct program_counter *pc);
void jump_counter(struct program_counter *pc, int address);
void init_program_counter(struct program_counter * pc, int Machine_Memory[MEMORY_SIZE][WORD_SIZE]);
void clear_accumulator(struct program_counter * pc);
void clear_multiplier_quotient(struct program_counter * pc);
void compute_instruction(struct program_counter *pc, int Machine_Memory[MEMORY_SIZE][WORD_SIZE]);
int get_address(struct program_counter *pc);
void set_address(struct program_counter *pc, int address, int Machine_Memory[MEMORY_SIZE][WORD_SIZE]);
int strip_opcode(struct program_counter *pc);
int accumulator_overflow(struct program_counter *pc);
int accumulator_empty(struct program_counter *pc);
long get_accumulator_value(struct program_counter *pc);
void set_accumulator_value(struct program_counter *pc, long value);
void print_pc(struct program_counter pc);

#endif
