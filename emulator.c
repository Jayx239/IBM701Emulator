#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "assembler.h"
#include "program_counter.h"
#include "bitmath.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
void save_instruction();
void import_file(char* buff);
void print_memory();
void import_instruction(char* instruction);
int execute_instruction();

int main(int argc, char* argv[])
{
    init_memory();
    init_program_counter(&pc, Machine_Memory);
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
    generate_opcodes(opcodes);
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
            fd = open(args[1],O_RDONLY);
            char buff[INSTRUCTION_SIZE];
            int read_len = 0;
            while((read_len = read(fd,buff , INSTRUCTION_SIZE)) > 0)
            {
                import_file(buff);
            }
        }
        if(memcmp(buff,"pm",2) == 0)
            print_memory();
        if(memcmp(buff,"ppc",3) == 0)
            print_pc(pc);
        if(memcmp(buff,"sv",2) == 0)
        { 
            save_instruction();
        }
        if(memcmp(buff,"ex",2) == 0)
        {
            execute_instruction();
        }


    }
}

void save_instruction()
{
    char instruction_string[WORD_SIZE];
    read(0,instruction_string,WORD_SIZE);
    int *byte_value;
    byte_value = (int*) malloc(WORD_SIZE*sizeof(int));
    byte_value_from_string(instruction_string,byte_value,WORD_SIZE);
    for(int i=0; i<WORD_SIZE; i++)
        Machine_Memory[pc.current_address][i] = byte_value[i];

    set_address(&pc,pc.current_address,Machine_Memory);
    free(byte_value);
    char* memory_value = (char*) malloc(WORD_SIZE);
    byte_value_to_string(Machine_Memory[pc.current_address], memory_value, WORD_SIZE);
        fprintf(stdout, "%d: %s\n",pc.current_address, memory_value);
    free(memory_value);

}


void import_file(char* file_name)
{
    int fd;
    char buff[50];
    fd = open(file_name,O_RDONLY);
    while(read(fd,buff,36))
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

    memcpy(&Machine_Memory[pc.current_address],instruction_decoded,INSTRUCTION_SIZE);;

    increment_counter(&pc);
    increment_counter(&pc);

}


/* 
 * Returns 0 on memory transfers
 * 
 */

int execute_instruction()
{
    int opcode = strip_opcode(&pc);
    long value_address, accum_val, new_accum_val;

    switch(opcode)
    {
        case 0b00000:
            set_address(&pc,pc.current_address,Machine_Memory);
            return 0;

        case 0b00001:
            set_address(&pc,get_address(&pc), Machine_Memory);
            //break;
            return 0;
        case 0b00010:
            if(accumulator_overflow(&pc))
                set_address(&pc,get_address(&pc), Machine_Memory);
            return 0;
        case 0b00011:
            if(accumulator_empty(&pc) >=0)
                set_address(&pc,get_address(&pc), Machine_Memory);
            return 0;
        case 0b00100:
            if(accumulator_empty(&pc) == 0) 
                set_address(&pc,get_address(&pc), Machine_Memory);
            return 0;
        case 0b00101:
            value_address = signed_byte_value(Machine_Memory[get_address(&pc)],WORD_SIZE);
            accum_val = get_accumulator_value(&pc);
            new_accum_val = accum_val - value_address;
            set_accumulator_value(&pc, new_accum_val);
            break;
        case 0b00110:
            value_address = signed_byte_value(Machine_Memory[get_address(&pc)],WORD_SIZE);
            value_address*=-1;
            set_accumulator_value(&pc,value_address);
            break;
        case 0b00111:
            value_address = signed_byte_value(Machine_Memory[get_address(&pc)], WORD_SIZE);
            if(value_address < 0)
                value_address*=-1;
            accum_val = get_accumulator_value(&pc);
            new_accum_val = accum_val - value_address;
            set_accumulator_value(&pc, new_accum_val);
            break;
        case 0b01001:
            value_address = signed_byte_value(Machine_Memory[get_address(&pc)], WORD_SIZE);
            accum_val = get_accumulator_value(&pc);
            new_accum_val = accum_val + value_address;
            set_accumulator_value(&pc, new_accum_val);
            break;
        case 0b01010:
            memcpy(&pc.accumulator, &Machine_Memory[get_address(&pc)], ACCUMULATOR_SIZE);
            break;
        case 0b01011:
            value_address = signed_byte_value(Machine_Memory[get_address(&pc)], WORD_SIZE);
            if(value_address < 0)
                value_address*=-1;
            accum_val = get_accumulator_value(&pc);
            new_accum_val = accum_val + value_address;
            set_accumulator_value(&pc, new_accum_val);
            break;
        case 0b01100:
            memcpy(&Machine_Memory[get_address(&pc)],pc.accumulator, ACCUMULATOR_SIZE);
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
        for(int j=0; j<INSTRUCTION_SIZE; j++)
            fprintf(stdout,"%d",Machine_Memory[i][j]);

        fprintf(stdout,"\n");
    }

}
