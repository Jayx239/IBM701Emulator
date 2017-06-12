#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program_counter.h"
#include "bitmath.h"

long long signed_byte_value(int bit_array[], int size)
{
    int is_signed = bit_array[size-1];
    
    /*if(is_signed)
    {
        for(int i=0; i<size; i++)
        {
            bit_array[i] = (bit_array[i] == 0) ? 1 : 0;
        }

        int carry = 1;
        for(int i=0; i<size; i++)
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
    }*/
     
    int pow = 1;
    long long output = 0;

    for(int i=0; i<size-1; i++)
    {
        output += bit_array[i]*pow;
        pow*=2;
    }

    if(is_signed)
        output*=-1;

    return output;
}

int* create_byte_value(long value, int out_size)
{
    int is_negative = value < 0 ? 1 : 0;
    if(is_negative)
        value*=-1;
    int *byte_array = malloc(sizeof(int)*out_size);
    int offset=1;
    for(int i=0; i<out_size; i++)
    {
        byte_array[i] = (value & offset) >> i;
        offset*=2;
    }

    byte_array[out_size-1] = is_negative;

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

void add_accumulator(struct program_counter *pc, long add_val)
{
    long init_val = signed_byte_value(pc->accumulator,ACCUMULATOR_SIZE);
    long new_val = init_val + add_val;
    memcpy(pc->accumulator,create_byte_value(new_val,ACCUMULATOR_SIZE),ACCUMULATOR_SIZE);
}

// returns remainder

int add_bit_array(int value_a[], int value_b[],int* output, int size)
{
    if(output != NULL)
        free(output);
    
    output = malloc(sizeof(int)*size);

    int carry = 0;
    for(int i=0; i<size; i++)
    {
        if(value_a[i]+value_b[i] == 0)
        {
            if(carry > 0)
            {
                output[i] = 1;
                carry--;
            }
            else
                output[i] = 0;
        }
        else if(value_a[i] == value_b[i] && value_a[i] == 1)
        {
            if(carry == 1)
            {
                output[i] = 1;
            }
            else
                output[i] =0;
            
            carry = 1;
        }
        else if(value_a[i] != value_b[i])
        {
            if(carry == 1)
            {
                output[i] = 0;
            }
            else
                output[i] = 1;
        
        }
    
    }

    if(carry)
    {
        return 1;
    }

    return 0;
}


// A - B
int subtract_bit_array(int value_a[], int value_b[], int* output, int size)
{
   return 0;

}

void and_bit_array(int value_a[], int value_b[], int *output, int size)
{
    for(int i=0; i<size; i++)
        if(value_a[i] == value_b[i] && value_a[i] == 1)
            output[i] = 1;
        else
            output[i] = 0;

}

void shift_bit_array(int* array, int size, int shift_amount, int shift_left)
{
    if(shift_left)
        for(int i=size-1; i>=0; i--)
        {
            if(i-shift_amount >=0)
                array[i] = array[i-shift_amount];
            else
                array[i] = 0;
        }
    else
        for(int i=0; i < size; i++)
            if((i + shift_amount) < size)
                array[i] = array[i+shift_amount];
            else
                array[i] = 0;

}
