#ifndef TASK2_H
#define TASK2_H

#include <stdint.h>
#include <stdbool.h>
#include "task1.h"
#include "main.h"

typedef struct {
    int pnumber;            // Page number
    _Bool pfault;           // Page fault occured?
    int fnumber;            // Frame number
    int paddress;           // Physical address
} task2_value_t;

void run_task2(task1_value_t task1_value, task2_value_t *task2_value, 
    page_table_entry_t *page_table, frame_t *frame_table, page_queue_t *page_queue, tlb_t *tlb);

#endif