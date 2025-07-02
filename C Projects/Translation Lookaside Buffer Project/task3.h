#ifndef TASK3_H
#define TASK3_H

#include <stdint.h>
#include <stdbool.h>
#include "task1.h"
#include "main.h"
#include "task2.h"

void run_task3(task1_value_t task1_value, task2_value_t *task2_value, page_table_entry_t *page_table, 
    frame_t *frame_table, page_queue_t *page_queue, tlb_t *tlb);

#endif