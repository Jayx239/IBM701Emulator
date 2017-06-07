#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "assembler.h"

int main(int argc, char* argv[])
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
}

struct opcode*  generate_opcodes()
{
    struct opcode opcodes[24];

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
    opcodes[2].key = "TROF";       //transfer on overflow
    opcodes[3].key = "TR+";        // Transfer on plus
    opcodes[4].key = "TR0";        // Transfer on 0  
    opcodes[5].key = "SUB";        // Subtract
    opcodes[6].key = "RSUB";       // Reset and subtract
    opcodes[7].key = "SUBAB";      // Subtract absolute value
    opcodes[8].key = "ADD";        // Add
    opcodes[9].key = "RADD";       // Reset and add
    opcodes[10].key = "ADDAB";     // Add absolute
    opcodes[11].key = "STORE";     // Store
    opcodes[12].key = "STOREA";    // Store address
    opcodes[13].key = "EXTR";      // Extract
    opcodes[14].key = "STOREMQ";   // Store MQ
    opcodes[15].key = "LOADMQ";    // Load MQ
    opcodes[16].key = "MPY";       // Multiply
    opcodes[17].key = "MPYROUND";  //Mutiply and Round
    opcodes[18].key = "DIV";       // Divide
    opcodes[19].key = "ROUND";     //Round
    opcodes[20].key = "LLEFT";     // Long left shift
    opcodes[21].key = "LRIGHT";    // Long right shift
    opcodes[22].key = "ALEFT";     // Accumulator left shift
    opcodes[23].key = "ARIGHT";    // Accumulator right shift

    return (struct opcode*) opcodes;
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
    for( i=0; i<8; i++)
    {
        printf("%d",(value & (1 << i)) >> i);
    }
}

struct opcode get_opcode(char* code, struct opcode* opcodes)
{
    for(int i=0; i<NUM_OPCODES; i++)
    {
        if(strcmp(opcodes[i]->key,code) == 0)
        {
            return opcodes[i];
        }
    }
    return (opcode) NULL;
}
