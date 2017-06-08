#include <stdio.h>
#include <stdlib.h>
#include "bitmath.h"

long signed_byte_value(int bit_array[], int size)
{
    int is_signed = bit_array[0];
    
    if(is_signed)
    {
        for(int i=1; i<size; i++)
        {
            bit_array[i] = (bit_array[i] == 0) ? 1 : 0;
        }

        int carry = 1;
        for(int i=1; i<size; i++)
        {
            if(bit_array[i] == 0)
            {
                bit_array[i] = 1;
                carry = 0;
                break;
            }
            else if(carry == 1)
            {
                bit_array[i] = 0;
                carry++;
            }
        }

        if(carry)
            fprintf(stderr,"Overflow, handler not implemented");
    }
    
    int pow = 1;
    long output = 0;

    for(int i=1; i<size; i++)
    {
        output += bit_array[i]*pow;
        pow*=2;
    }

    return output;
}

int* create_byte_value(long value, int out_size)
{
    int *byte_array = malloc(sizeof(int)*out_size);
    int offset=1;
    for(int i=0; i<out_size; i++)
    {
        byte_array[i] = (value & offset) >> i;
        offset*=2;
    }

    return byte_array;
    
}

void byte_value_to_string(int value[],char* output, int size)
{
    for(int i=0; i<size; i++)
        output[i] = (char) (value[i]+48);
}

void byte_value_from_string(char* input, int* output,int length)
{
    for(int i=0; i<length; i++)
       output[i] = (input[i] == '0') ? 0 : 1;
    
}
