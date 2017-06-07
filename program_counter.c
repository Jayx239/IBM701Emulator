#include "program_counter.h"
#include "bitmath.h"

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
        printf(stderr,"Program counter reached end of memory");
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
        printf(stderr,"Program counter reached beginning of memory");
}

void jump_counter(struct program_counter *pc, int address)
{
    pc->offset = 0;
    if(address > -1 && address < 2048)
        pc->current_address = address;
    else
        printf(stderr,"Program counter jump address out of range");
}

void init_program_counter(struct program_counter * pc, int Machine_Memory[][]);
{
    set_address(pc,0,Machine_Memory);
    clear_accumulator(pc);
    clear_multiplier_quotient(pc);

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

void compute_instruction(struct program_counter *pc, int Machine_Memory[][])
{
    int output[18];
    if(pc->offset == 1)
    {
        for(int i=INSTRUCTION_SIZE-1; i>=0; i--)
        {
            output[i] = Machine_Memory[i];
        }
    }
    else
    {
        for(int i=INSTRUCTION_SIZE-1; i>=0; i--)
        {
            output[i] = Machine_Memory[i+INSTRUCTION_SIZE];
        }
    }

    pc->instruction = output;
}

int get_address(struct program_counter *pc)
{
    int address = 0;
    int pow = 1;
    for(int i=0; i<INSTRUCTION_SIZE-OPCODE_SIZE; i++)
    {
        address += pc->instruction[i]*pow;
        pow*=2;
    }

    return address;
}

void set_address(struct program_counter *pc, int address, Machine_Memory[MEMORY_SIZE][WORD_SIZE])
{
    pc->current_address = address;
    pc->address_word = Machine_Memory[address];
    compute_instruction(pc,Machine_Menmory);
}

int strip_opcode(struct program_counter *pc)
{

    int pow = 1;
    int output = 0;
    for(int i=0; i<OPCODE_SIZE; i++)
    {
        output+= pow*pc->instruction[i];
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

    if(i == 1)
        is_positive*=-1;


    for(int i=2; i<ACCUMULATOR_SIZE; i++)
        if(pc->accumulator[i] == 1)
        {
            is_zero = 0;
            break;
        }

    if(is_zero)
        ruturn 0;
    
    return is_positive;
}

long get_accumulator_value(struct program_counter *pc)
{
    int value[ACCUMULATOR_SIZE-2];
    value[0] = pc->accumulator[0];
    for(int i=3; i<ACCUMULATOR_SIZE; i++)
        value[i-2] = pc->accumulator[i];

    return signed_byte_value(value);

}

void set_accumulator_value(struct program_counter *pc, long new_value)
{
    pc->accumulator = create_byte_value(new_value,ACCUMULATOR_SIZE);
}


