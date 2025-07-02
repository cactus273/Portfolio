/**
 * Program to read logical addresses from a file and print their page numbers and offsets.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task1.h"
#include "main.h"

// Generates a bitmask with the lowest 'input_bits' bits set to 1.
unsigned int generate_bitmask(int input_bits) {
    return (1u << input_bits) - 1;
}

// Function to obtain the page number and offset from a logical address
void run_task1(int input, task1_value_t *task1_value) {
    *task1_value = (task1_value_t){.laddress = -1, .pnumber = -1, .poffset = -1};

    // Identify page number and offset
    unsigned int masked_value = input & generate_bitmask(PARSED_BITS);
    unsigned int page_number_bits = (masked_value >> OFFSET_BITS) & generate_bitmask(PAGE_NUMBER_BITS);
    unsigned int offset_bits = masked_value & generate_bitmask(OFFSET_BITS);

    task1_value->laddress = input;
    task1_value->pnumber = (int)page_number_bits;
    task1_value->poffset = (int)offset_bits;

    return;
}