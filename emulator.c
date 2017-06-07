#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "assembler.h"
#include "program_counter.h"
#include "bitmath.h"

#define INSTRUCTION_SIZE 18
#define OPCODE_SIZE 6
#define MEMORY_SIZE 2048

int run = 1;
struct opcode opcodes[NUM_OPCODES];
int Machine_Memory[MEMORY_SIZE][WORD_SIZE];
int accumulator[38];
int multiplier_qotient[36];
struct program_counter pc;


void* init_memory();
void* command_reader(char* args[]);
int import_file(char* buff);
void print_memory();

int main(int argc, char* argv[])
{
    init_memory();
    init_program_counter(&pc);
    command_reader(argv); 
    return 0;
}

void* init_memory()
{
    for(int i=0; i<MEMORY_SIZE; i++)
        for(int j=0; j<WORD_SIZE; j++)
            Machine_Memory[i][j] = 0;
}

void* command_reader(char* args[])
{
    opcodes = generate_opcodes();
    char buff[50];
    int fd;

    while(run == 1)
    {
        printf("Enter a command: ");
        fflush(stdout);
        read(0,buff, 50);

        if(memcmp(buff,"quit",4) == 0)
            run = 0;
        if(memcmp(buff,"file",4) == 0)
        {
            fd = open(args[1],"r");
            char buff[INSTRUCTION_SIZE];
            while((buff = readn(fd,buff , INSTRUCTION_SIZE)) > 0)
            {
                import_file(buff);
            }
        }
        if(memcmp(buff,"pm",2))
            print_memory();
    }
}

void import_file(char* file_name)
{
    FILE *fp;
    char buff[50];
    fp = fopen(file_name,"r");
    while(read(fp,buff,36))
    {
        import_instruction(buff);
    }
    

}

void import_instruction(char* instruction)
{
    int instruction_decoded[36];
    for(int i=0; i<INSTRUCTION_SIZE; i++)
        if(instruction[i] == '1')
            instruction_decoded[i] = 1;
        else
            instruction_decoded[i] = 0;
    
    Machine_Memory[pc->current_address] = instruction_decoded;
    
    increment_counter(pc);
    increment_counter(pc);

}


/* 
 * Returns 0 on memory transfers
 * 
 */

int execute_instruction(char* instruction)
{
    int opcode = strip_opcode(&pc);
    
    switch(opcode)
    {
        case 0b00000:
            set_address(&pc,pc.address,Machine_Memory);
            return 0;
            
        case 0b00001:
            set_address(&pc,get_address(&pc));
            //break;
            return 0;
        case 0b00010:
            if(accumulator_overflow(&pc))
                set_address(&pc,get_address(&pc));
            return 0;
        case 0b00011:
            if(accumulator_empty(&pc) >=0)
                set_address(&pc,get_address(&pc));
            return 0;
        case 0b00100:
            if(accumulator_empty(&pc) == 0) 
                set_address(&pc,get_address(&pc));
            return 0;
        case 0b00101:
            long value_address = signed_byte_value(Machine_Memory[get_address(&pc)]);
            long accum_val = get_accumulator_value(&pc);
            long new_accum_val = accum_val - value_address;
            set_accumulator_value(&pc, new_accum_val);
            break;
        case 0b00110:
            long value_address = signed_byte_value(Machine_Memory[get_address(&pc)]);
            value_address*=-1;
            set_accumulator(&pc,value_address);
            break;
        case 0b00111:
            long value_address = signed_byte_value(Machine_Memory[get_address(&pc)]);
            if(value_address < 0)
                value_address*=-1;
            long accum_val = get_accumulator_value(&pc);
            long new_accum_val = accum_val - value_address;
            set_accumulator(&pc, new_accum_val);
            break;
        case 0b01001:
            long value_address = signed_byte_value(Machine_Memory[get_address(&pc)]);
            long accum_val = get_accumulator_value(&pc);
            long new_accum_val = accum_val + value_address;
            set_accumulator_value(&pc, new_accum_val);
            break;
        case 0b01010:
            pc.accumulator = Machine_Memory[get_address(&pc)];
            break;
        case 0b01011:
            long value_address = signed_byte_value(Machine_Memory[get_address(&pc)]);
            if(value_address < 0)
                value_address*=-1;
            long accum_val = get_accumulator_value(&pc);
            long new_accum_val = accum_val + value_address;
            set_accumulator(&pc, new_accum_val);
            break;
        case 0b01100:
            Machine_Memory[get_address(&pc)] = pc.accumulator;
            break;
        case 0b001101:
            
            break;
        case 0b101101:

            break;
        case 0b01110:

            break;
        case 0b01111:

            break;
        case 0b10000:

            break;
        case 0b10001:

            break;
        case 0b10010:

            break;
        case 0b10011:

            break;
        case 0b10100:

            break;
        case 0b10101:

            break;
        case 0b10110:

            break;
        case 0b10111:

            break;
        default:
            break;
    }



}

void print_memory()
{
    for(int i=0; i<MEMORY_SIZE; i++)
    {
        for(int j=0; j<INTRUCTION_SIZE; j++)
            printf(stdout,"%d",Machine_Memory);

        printf(stdout,"\n");
    }

}
