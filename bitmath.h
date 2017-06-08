#ifndef BITMATH_H
#define BITMATH_H

long signed_byte_value(int value[], int size);
int* create_byte_value(long value, int out_size);
void byte_value_to_string(int value[],char* output, int size);
void byte_value_from_string(char* input, int* output, int length);

#endif
