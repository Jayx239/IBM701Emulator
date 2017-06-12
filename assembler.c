#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "assembler.h"

/*int main(int argc, char* argv[])
{   
    int in_fd = 0;
    if(argc > 1)
        in_fd = (int) fopen(argv[1],"r");
    //else
        //return 0;

    struct opcode opcodes[24];
    
    generate_opcodes(opcodes);
    display_opcodes(opcodes);
    return 0;
}*/

void  generate_opcodes(struct opcode opcodes[])
{
    opcodes[0].value = 0b00000;
    opcodes[1].value = 0b00001;
    opcodes[2].value = 0b00010;
    opcodes[3].value = 0b00011;
    opcodes[4].value = 0b00100;
    opcodes[5].value = 0b00101;
    opcodes[6].value = 0b00110;
    opcodes[7].value = 0b00111;
    opcodes[8].value = 0b01001;
    opcodes[9].value = 0b01010;
    opcodes[10].value = 0b01011;
    opcodes[11].value = 0b01100;
    opcodes[12].value = 0b001101;
    opcodes[13].value = 0b101101;
    opcodes[14].value = 0b01110;
    opcodes[15].value = 0b01111;
    opcodes[16].value = 0b10000;
    opcodes[17].value = 0b10001;
    opcodes[18].value = 0b10010;
    opcodes[19].value = 0b10011;
    opcodes[20].value = 0b10100;
    opcodes[21].value = 0b10101;
    opcodes[22].value = 0b10110;
    opcodes[23].value = 0b10111;

    opcodes[0].key = "STOP";         //stop
    opcodes[1].key = "TR";         //transfer
    opcodes[2].key = "TR OV";       //transfer on overflow
    opcodes[3].key = "TR +";        // Transfer on plus
    opcodes[4].key = "TR 0";        // Transfer on 0  
    opcodes[5].key = "SUB";        // Subtract
    opcodes[6].key = "R SUB";       // Reset and subtract
    opcodes[7].key = "SUB AB";      // Subtract absolute value
    opcodes[8].key = "ADD";        // Add
    opcodes[9].key = "R ADD";       // Reset and add
    opcodes[10].key = "ADD AB";     // Add absolute
    opcodes[11].key = "STORE";     // Store
    opcodes[12].key = "STORE A";    // Store address
    opcodes[13].key = "EXTR";      // Extract
    opcodes[14].key = "STORE MQ";   // Store MQ
    opcodes[15].key = "LOAD MQ";    // Load MQ
    opcodes[16].key = "MPY";       // Multiply
    opcodes[17].key = "MPY ROUND";  //Mutiply and Round
    opcodes[18].key = "DIV";       // Divide
    opcodes[19].key = "ROUND";     //Round
    opcodes[20].key = "L LEFT";     // Long left shift
    opcodes[21].key = "L RIGHT";    // Long right shift
    opcodes[22].key = "A LEFT";     // Accumulator left shift
    opcodes[23].key = "A RIGHT";    // Accumulator right shift

    opcodes[0].key_size = 4;
    opcodes[1].key_size = 2;
    opcodes[2].key_size = 5;
    opcodes[3].key_size = 4;
    opcodes[4].key_size = 4;
    opcodes[5].key_size = 3;
    opcodes[6].key_size = 5;
    opcodes[7].key_size = 6;
    opcodes[8].key_size = 3;
    opcodes[9].key_size = 5;
    opcodes[10].key_size = 6;
    opcodes[11].key_size = 5;
    opcodes[12].key_size = 7;
    opcodes[13].key_size = 4;
    opcodes[14].key_size = 8;
    opcodes[15].key_size = 7;
    opcodes[16].key_size = 3;
    opcodes[17].key_size = 9;
    opcodes[18].key_size = 3;
    opcodes[19].key_size = 5;
    opcodes[20].key_size = 6;
    opcodes[21].key_size = 7;
    opcodes[22].key_size = 6;
    opcodes[23].key_size = 7;

    
}

void create_instruction(struct program_counter *pc, int* Machine_Memory, struct opcode command_opcode,int address)
{
    int *instruction = &Machine_Memory[pc->current_address];

    int pow = 1;
    for(int i=0; i<OPCODE_SIZE; i++)
    {
        instruction[i] = (command_opcode.value & pow ) >> i;
        pow*=2;
    }
    
    int* address_array = create_byte_value(address,INSTRUCTION_SIZE-OPCODE_SIZE);
    
    for(int i=OPCODE_SIZE; i<INSTRUCTION_SIZE; i++)
        instruction[i] = address_array[i-OPCODE_SIZE];
    
    for(int i=INSTRUCTION_SIZE; i<WORD_SIZE; i++)
        instruction[i] = 0;
}

void display_opcodes(struct opcode opcodes[])
{
    int i=0;
    printf("Opcode : Binary value\n");

    for( i=0; i<NUM_OPCODES; i++)
    {
        
        printf("%s : ",opcodes[i].key);
        print_binary(opcodes[i].value);
        printf("\n");
    }
}

void print_binary(int value)
{
    int i=0;
    for( i=7; i>=0; i--)
    {
        printf("%d",(value & (1 << i)) >> i);
    }
}

struct opcode get_opcode(char* code, struct opcode* opcodes)
{
    int total_length = 0;
    int temp_length = 0;
    struct opcode out_opcode;
    out_opcode.value = -1;
    out_opcode.key = "null";

    for(int i=0; i<NUM_OPCODES; i++)
    {
        if(sizeof(code) < opcodes[i].key_size)
            continue;

        temp_length = 0;
        for(int j=0; j<opcodes[i].key_size; j++)
        {
            if(opcodes[i].key[j] == code[j])
                temp_length++;
            else
                break;
        }

        if(temp_length > total_length && temp_length == opcodes[i].key_size)
        {
            total_length = temp_length;
            out_opcode = opcodes[i];
        }
    }
    
    return out_opcode;
}
