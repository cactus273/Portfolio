#ifndef TASK4_H
#define TASK4_H

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "task1.h"
#include "task2.h"

void run_task4(task1_value_t task1_value, task2_value_t *task2_value, tlb_t *tlb, 
    page_table_entry_t *page_table, frame_t *frame_table, page_queue_t *page_queue);

void flush_tlb_entry(tlb_t *tlb, int page_number);

#endif