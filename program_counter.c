#include <stdio.h>
#include <stdlib.h>
#include "program_counter.h"
#include "bitmath.h"
#include "assembler.h"
#include <string.h>

void increment_counter(struct program_counter *pc)
{
    if(pc->offset == 0)
    {
        pc->offset = 1; 
        return;
    }

    if(pc->current_address < 2047)
    {
        pc->current_address+=1;
        pc->offset = 0;
    }
    else
        fprintf(stderr,"Program counter reached end of memory");
}

void decrement_counter(struct program_counter *pc)
{
    if(pc->offset == 1)
    {
        pc->offset = 0;
        return;
    }

    if(pc->current_address > 0)
    {
        pc->offset == 1;
        pc->current_address--;

    }
    else
        fprintf(stderr,"Program counter reached beginning of memory");
}

void jump_counter(struct program_counter *pc, int address)
{
    pc->offset = 0;
    if(address > -1 && address < 2048)
        pc->current_address = address;
    else
        fprintf(stderr,"Program counter jump address out of range");
}

void init_program_counter(struct program_counter * pc, int Machine_Memory[MEMORY_SIZE][WORD_SIZE])
{
    set_address(pc,0,Machine_Memory);
    clear_accumulator(pc);
    clear_multiplier_quotient(pc);
    compute_instruction(pc, Machine_Memory);
}

void clear_accumulator(struct program_counter * pc)
{
    for(int i=0; i<ACCUMULATOR_SIZE; i++)
        pc->accumulator[i] = 0;

}

void clear_multiplier_quotient(struct program_counter * pc)
{
    for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
        pc->multiplier_quotient[i] = 0;

}

void compute_instruction(struct program_counter *pc, int Machine_Memory[MEMORY_SIZE][WORD_SIZE])
{
    if(pc->offset == 1)
    {
        for(int i=INSTRUCTION_SIZE-1; i>=0; i--)
        {
            pc->instruction[i] = Machine_Memory[pc->current_address][i];
        }
    }
    else
    {
        for(int i=INSTRUCTION_SIZE-1; i>=0; i--)
        {
            pc->instruction[i] = Machine_Memory[pc->current_address][i+INSTRUCTION_SIZE];
        }
    }
    
}

int get_address(struct program_counter *pc)
{
    int address = 0;
    int pow = 1;
    int offset = pc->offset == 0 ? INSTRUCTION_SIZE : 0;

    for(int i=OPCODE_SIZE; i<INSTRUCTION_SIZE; i++)
    {
        address += pc->instruction[i+offset]*pow;
        pow*=2;
    }

    return address;
}

void set_address(struct program_counter *pc, int address, int Machine_Memory[MEMORY_SIZE][WORD_SIZE])
{
    pc->current_address = address;
    for(int i=0; i<WORD_SIZE; i++)
        pc->address_word[i] = Machine_Memory[address][i];

    compute_instruction(pc,Machine_Memory);
}

int strip_opcode(struct program_counter *pc)
{

    int pow = 1;
    int output = 0;
    int offset = pc->offset ? INSTRUCTION_SIZE : 0;
    for(int i=0; i<OPCODE_SIZE; i++)
    {
        output+= pow*pc->instruction[i+offset];
        pow*=2;
    }

    return output;
}

int accumulator_overflow(struct program_counter *pc)
{
    if(pc->accumulator[0] == 1 || pc->accumulator[1] == 1)
        return 1;

    return 0;
}

int accumulator_empty(struct program_counter *pc)
{
    int is_positive = 1;
    int is_zero = 1;

    if(pc->accumulator[0] == 1)
        is_positive*=-1;


    for(int i=2; i<ACCUMULATOR_SIZE; i++)
        if(pc->accumulator[i] == 1)
        {
            is_zero = 0;
            break;
        }

    if(is_zero)
        return 0;
    
    return is_positive;
}

long get_accumulator_value(struct program_counter *pc)
{
    //int value[ACCUMULATOR_SIZE];
    //value[0] = pc->accumulator[0];
    //for(int i=0; i<ACCUMULATOR_SIZE; i++)
        //value[i] = pc->accumulator[i];

    return signed_byte_value(pc->accumulator, ACCUMULATOR_SIZE);

}

void set_accumulator_value(struct program_counter *pc, long new_value)
{
    int *temp_val = create_byte_value(new_value,ACCUMULATOR_SIZE);
    memcpy(pc->accumulator,temp_val,ACCUMULATOR_SIZE*sizeof(int));

    free(temp_val);
}

void print_pc(struct program_counter pc)
{
    
    printf("Current Address: %d\n", pc.current_address);
    printf("Offset: %d\n", pc.offset);

    char* address_word = malloc(WORD_SIZE);
    byte_value_to_string(pc.address_word, address_word, WORD_SIZE);
    printf("Address Word: %s\n", address_word);
    
    char* instruction_string = malloc(INSTRUCTION_SIZE);
    byte_value_to_string(pc.instruction, instruction_string, INSTRUCTION_SIZE);
    printf("Instruction: %s\n", instruction_string);
    
    byte_value_to_string(pc.multiplier_quotient, address_word, WORD_SIZE);
    printf("Multiplier Quoticent: %s\n", address_word);
    
    char* accumulator_string = (char*) malloc(ACCUMULATOR_SIZE);
    byte_value_to_string(pc.accumulator, accumulator_string, ACCUMULATOR_SIZE);
    printf("Accumulator: %s\n\n", accumulator_string);    

    free(address_word);
    free(instruction_string);
}

/*void refresh_pc(struct program_counter *pc, int Machine_Memory[MEMORY_SIZE][WORD_SIZE])
{
   compute_instruction(pc, Machine_Memory);

}*/
