#ifndef TASK1_H
#define TASK1_H

#include <stdint.h>

typedef struct {
    int laddress;   // Logical address  
    int pnumber;    // Page number
    int poffset;    // Offset
} task1_value_t;

unsigned int generate_bitmask(int num_bits);
void run_task1(int input, task1_value_t *task1_value);

#endif