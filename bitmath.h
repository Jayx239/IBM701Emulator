#ifndef BITMATH_H
#define BITMATH_H

long long signed_byte_value(int value[], int size);
int* create_byte_value(long value, int out_size);
void byte_value_to_string(int value[],char* output, int size);
void byte_value_from_string(char* input, int* output, int length);
void add_accumulator(struct program_counter *pc, long add_val);
int add_bit_array(int value_a[], int value_b[],int* output, int size);
int subtract_bit_array(int value_a[], int value_b[], int* output, int size);
void and_bit_array(int value_a[], int value_b[], int* output, int size);
void shift_bit_array(int* array, int size, int shift_amount, int shift_left);

#endif
