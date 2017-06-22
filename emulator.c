#include <signal.h>
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
#include <setjmp.h>
#include <ctype.h>

#define INSTRUCTION_SIZE 18
#define OPCODE_SIZE 6
#define MEMORY_SIZE 2048

int run = 1;
struct opcode opcodes[NUM_OPCODES];
int Machine_Memory[MEMORY_SIZE][WORD_SIZE];
int accumulator[38];
int multiplier_qotient[36];
struct program_counter pc;
static jmp_buf file_in_jmp;
int PPC_Ex_Mode = 1;
int run_prog = 1;

void init_memory();
void command_reader(char* args[]);
void save_instruction();
void import_file(char* buff);
void print_memory();
void import_instruction(char* instruction);
int execute_instruction();
void save_opcode_instruction(struct opcode inst_opcode, char* buff);
void save_file(char *file_buff);

static void sigstp_handler(int sig);

int main(int argc, char* argv[])
{
    if(signal(SIGTSTP, sigstp_handler) == SIG_ERR)
        printf("Error setting up signal handler!\n");

    init_memory();
    init_program_counter(&pc, Machine_Memory);
    command_reader(argv); 
    return 0;
}

void init_memory()
{
    for(int i=0; i<MEMORY_SIZE; i++)
        for(int j=0; j<WORD_SIZE; j++)
            Machine_Memory[i][j] = 0;
}

void command_reader(char* args[])
{
    generate_opcodes(opcodes);
    char buff[50];
    int read_size = 0;
    int fd;
    struct opcode next_opcode;
    while(run == 1)
    {
        if(setjmp(file_in_jmp) == 1)
            printf("Invalid file name\n");

        printf("Enter a command: ");
        fflush(stdout);
        read_size = read(0,buff, 50);

        if(memcmp(buff,"quit",4) == 0)
            run = 0;
        else if(memcmp(buff,"file",4) == 0)
        {
            printf("Enter file name (precede with ./ if local path): ");
            fflush(stdout);

            char file_buff[50];
            int read_len = 0;
            read_len = read(0,file_buff , 50);
            file_buff[read_len-1] = '\0';
            import_file(file_buff);
        }
        else if(memcmp(buff,"pm",2) == 0)
            print_memory();
        else if(memcmp(buff,"ppc",3) == 0)
            print_pc(pc);
        else if(memcmp(buff,"sv",2) == 0)
        { 
            save_instruction();
        }
        else if(memcmp(buff,"ex",2) == 0)
        {
            if(execute_instruction() == 1)
                increment_counter(&pc);
            set_address(&pc,pc.current_address,Machine_Memory);
            
            if(PPC_Ex_Mode == 1)
                print_pc(pc);
        }
        else if(memcmp(buff,"help",4) == 0)
        {
            printf("Help menu ------------\nppc - print program pointer details\nsv - save next binary input into current memory location\nex - executes instruction in current register\nhelp - show this menu\npm - print memory contents from -10 to +10 address's around the current address\nop - prints a list of the accepted opcodes. To enter an opcode just type the opcode and the value of the instruction\nfile - opens up a file, for relative directory type ./file_name\nquit - exit the emulator\nsave - save contents of the memory registers\njump # - this command followed by a number will jump the program counter to that memory address\nrun - executes instructions continuously displaying the register contents each execution. Type ctrl-z to stop execution.\nsm - set ex mode, 1 prints ppc each time an execution is done, 0 does not\n\n");
        }
        else if(memcmp(buff,"op",2) == 0)
        {
            display_opcodes(opcodes);
        }
        else if(memcmp(buff,"save",4) == 0)
        {
            printf("Enter file name: ");
            fflush(stdout);
            
            char file_buff[50];
            int read_len = 0;
            read_len = read(0,file_buff , 50);
            file_buff[read_len-1] = '\0';
            save_file(file_buff);

        }
        else if(memcmp(buff,"run",3) == 0)
        {
            run_prog = 1;
            
            while(run_prog)
            {
                if(execute_instruction() == 1)
                    increment_counter(&pc);
                set_address(&pc,pc.current_address,Machine_Memory);
                print_pc(pc);
                sleep(1);
            }
            
        }
        else if(memcmp(buff,"jump",4) == 0)
        {
            int next_address = atoi(&buff[5]);
            set_address(&pc, next_address, Machine_Memory);
        }
        else if(memcmp(buff,"sm",2) == 0)
        {
            int mode_value = atoi(&buff[3]);
            PPC_Ex_Mode = mode_value;
        }
        else
        {
            next_opcode = get_opcode(buff,opcodes,read_size);
            if(next_opcode.value != -1)
            {
                save_opcode_instruction(next_opcode,buff);
                set_address(&pc,pc.current_address,Machine_Memory);
            }
        }



    }
}

void save_file(char* file_buff)
{
    FILE *fp  = fopen(file_buff,"w");
    for(int i=0; i<MEMORY_SIZE; i++)
    {
        for(int j=0; j<WORD_SIZE; j++)
            fprintf(fp,"%d",Machine_Memory[i][j]);
        if(i != MEMORY_SIZE-1)
            fprintf(fp,"\n");
    }

    fclose(fp);
}

void save_opcode_instruction(struct opcode inst_opcode, char* buff)
{
    char* output = &buff[inst_opcode.key_size+1];
    int address = atoi(output);
    create_instruction(&pc,&Machine_Memory[pc.current_address][0],inst_opcode,address);
    jump_counter(&pc,pc.current_address);

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
    char buff[WORD_SIZE+1];
    fd = open(file_name,O_RDONLY);

    if(fd < 0)
        longjmp(file_in_jmp,1);

    while(read(fd,&buff,WORD_SIZE+1) > 0)
    {
        import_instruction(buff);
    }

    close(fd);

}

void import_instruction(char* instruction)
{
    int instruction_decoded[WORD_SIZE];
    for(int i=0; i<INSTRUCTION_SIZE; i++)
        if(instruction[i] == '1')
            instruction_decoded[i] = 1;
        else
            instruction_decoded[i] = 0;

    memcpy(&Machine_Memory[pc.current_address],instruction_decoded,WORD_SIZE);;

    increment_counter(&pc);

}


/* 
 * Returns 0 on memory transfers
 * 
 */

int execute_instruction()
{
    int opcode = strip_opcode(&pc);
    long long value_address, accum_val, new_accum_val,mq_value,multiplicand,dividend,remainder;
    int* temp_multiple_val;
    switch(opcode)
    {
        case 0b00000:
            set_address(&pc,pc.current_address,Machine_Memory);
            return 0;

        case 0b00001:
            set_address(&pc,get_address(&pc), Machine_Memory);
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
            memcpy(&Machine_Memory[get_address(&pc)],&pc.accumulator[OPCODE_SIZE],ADDRESS_SIZE);
            break;
        case 0b101101:
            and_bit_array(Machine_Memory[get_address(&pc)],pc.accumulator,&pc.accumulator[0], WORD_SIZE);
            break;
        case 0b01110:
            memcpy(&Machine_Memory[get_address(&pc)], pc.multiplier_quotient, ACCUMULATOR_SIZE);
            break;
        case 0b01111:
            memcpy(&pc.multiplier_quotient, &Machine_Memory[get_address(&pc)], ACCUMULATOR_SIZE);
            break;
        case 0b10000:
            mq_value = signed_byte_value(pc.multiplier_quotient,MULTIPLIER_QUOTIENT_SIZE);
            multiplicand = signed_byte_value(Machine_Memory[get_address(&pc)],ADDRESS_SIZE);
            new_accum_val = mq_value*multiplicand;
            temp_multiple_val = create_byte_value(new_accum_val,ACCUMULATOR_SIZE+MULTIPLIER_QUOTIENT_SIZE);
            for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
                pc.multiplier_quotient[i] = temp_multiple_val[i];
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                pc.accumulator[i] = temp_multiple_val[i+MULTIPLIER_QUOTIENT_SIZE];
            free(temp_multiple_val);
            break;
        case 0b10001:
            mq_value = signed_byte_value(pc.multiplier_quotient,MULTIPLIER_QUOTIENT_SIZE);
            multiplicand = signed_byte_value(Machine_Memory[get_address(&pc)],WORD_SIZE);
            new_accum_val = mq_value*multiplicand;
            temp_multiple_val = create_byte_value(new_accum_val,ACCUMULATOR_SIZE+MULTIPLIER_QUOTIENT_SIZE);
            for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
                pc.multiplier_quotient[i] = 0;
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                pc.accumulator[i] = temp_multiple_val[i+MULTIPLIER_QUOTIENT_SIZE];
            free(temp_multiple_val);

            break;
        case 0b10010:
            dividend = signed_byte_value(Machine_Memory[get_address(&pc)],WORD_SIZE);
            accum_val = signed_byte_value(pc.accumulator,ACCUMULATOR_SIZE)<<38;
            accum_val += signed_byte_value(pc.multiplier_quotient,MULTIPLIER_QUOTIENT_SIZE);
            new_accum_val/=dividend;
            remainder = accum_val%dividend;
            temp_multiple_val = create_byte_value(new_accum_val,MULTIPLIER_QUOTIENT_SIZE);
            for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
                pc.multiplier_quotient[i] = temp_multiple_val[i];

            free(temp_multiple_val);

            temp_multiple_val = create_byte_value(remainder,ACCUMULATOR_SIZE);
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                pc.accumulator[i] = temp_multiple_val[i];
            free(temp_multiple_val);
            break;
        case 0b10011:
            if(pc.multiplier_quotient[MULTIPLIER_QUOTIENT_SIZE-1] == 1)
                accum_val = signed_byte_value(pc.accumulator,ACCUMULATOR_SIZE);
            new_accum_val = accum_val+1;
            temp_multiple_val = create_byte_value(new_accum_val,ACCUMULATOR_SIZE);
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                pc.accumulator[i] = temp_multiple_val[i];
            free(temp_multiple_val);
            break;
        case 0b10100:
            temp_multiple_val = (int*) malloc((ACCUMULATOR_SIZE+MULTIPLIER_QUOTIENT_SIZE)*sizeof(int));
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                temp_multiple_val[i] = pc.accumulator[i];
            for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
                temp_multiple_val[ACCUMULATOR_SIZE + i] = pc.multiplier_quotient[i];


            shift_bit_array(temp_multiple_val,ACCUMULATOR_SIZE+MULTIPLIER_QUOTIENT_SIZE,get_address(&pc),0);
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                pc.accumulator[i] = temp_multiple_val[i];
            for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
                pc.multiplier_quotient[i] = temp_multiple_val[ACCUMULATOR_SIZE + i];

            free(temp_multiple_val);
            break;
        case 0b10101:
            temp_multiple_val = (int*) malloc((ACCUMULATOR_SIZE+MULTIPLIER_QUOTIENT_SIZE)*sizeof(int));
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                temp_multiple_val[i] = pc.accumulator[i];
            for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
                temp_multiple_val[ACCUMULATOR_SIZE + i] = pc.multiplier_quotient[i];


            shift_bit_array(temp_multiple_val,ACCUMULATOR_SIZE+MULTIPLIER_QUOTIENT_SIZE,get_address(&pc),1);
            for(int i=0; i<ACCUMULATOR_SIZE; i++)
                pc.accumulator[i] = temp_multiple_val[i];
            for(int i=0; i<MULTIPLIER_QUOTIENT_SIZE; i++)
                pc.multiplier_quotient[i] = temp_multiple_val[ACCUMULATOR_SIZE + i];

            free(temp_multiple_val);

            break;
        case 0b10110:
            shift_bit_array(&pc.accumulator[0],ACCUMULATOR_SIZE,get_address(&pc),0);
            break;
        case 0b10111:
            shift_bit_array(&pc.accumulator[0],ACCUMULATOR_SIZE,get_address(&pc),1);
            break;
        default:
            break;
    }

    return 1;

}

void print_memory()
{
    int current_address = pc.current_address;
    int start_address = current_address - 10 > 0 ? current_address-10 : 0;
    int stop_address = current_address + 10 < MEMORY_SIZE ? current_address + 10 : MEMORY_SIZE;

    for(int i=start_address; i<stop_address; i++)
    {
        if(i == current_address)
            fprintf(stdout,"pc-->");
        for(int j=0; j<INSTRUCTION_SIZE; j++)
            fprintf(stdout,"%d",Machine_Memory[i][j]);
        fprintf(stdout," ");
        for(int j=INSTRUCTION_SIZE; j<WORD_SIZE; j++)
            fprintf(stdout,"%d",Machine_Memory[i][j]);
        fprintf(stdout,"\n");
    }

}

void sigstp_handler(int sig)
{
    run_prog = 0;
    if(signal(SIGTSTP, sigstp_handler) == SIG_ERR)
        printf("Error setting up signal handler!\n");
}
