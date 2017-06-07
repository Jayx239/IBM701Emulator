#include "bitmath.h"

long signed_byte_value(int value[])
{
    int is_signed = value[0];
    
    if(is_signed)
    {
        for(int i=1; i<sizeof(bit_array); i++)
        {
            bit_array[i] != bit_array[i];
        }

        int carry = 1;
        for(int i=1; i<sizeof(bit_array); i++)
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
            printf(stderr,"Overflow, handler not implemented");
    }
    
    int pow = 1;
    long output = 0;

    for(int i=1; i<sizeof(bit_array); i++)
    {
        output += bit_array[i]*pow;
        pow*=2;
    }

    return output;
}

int* create_byte_value(long value, int out_size)
{
    int byte_array[out_size];
    int offset=1;
    for(int i=0; i<out_size; i++)
    {
        byte_array[i] = (value & offset) >> i;
        offset*=2;
    }

    return byte_array;
    
}
