/**
 * Program to implement page table and allocate pages to frames.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "task2.h"
#include "task3.h"

void run_task2(task1_value_t task1_value, task2_value_t *task2_value, page_table_entry_t *page_table, 
    frame_t *frame_table, page_queue_t *page_queue, tlb_t *tlb) {
    *task2_value = (task2_value_t){.pnumber = 0, .pfault = false, .fnumber = -1, .paddress = -1};
    task2_value->pnumber = task1_value.pnumber;

    if (page_table[task2_value->pnumber].is_present) {
        // Page is already present in the frame table
        task2_value->pfault = false;
        task2_value->fnumber = page_table[task2_value->pnumber].frame_number; // Get frame number
        task2_value->paddress = (task2_value->fnumber << OFFSET_BITS) | task1_value.poffset; // Calculate physical address
    } else {
        // Page fault occurred, allocate a new frame
        task2_value->pfault = true;
        for (int i = 0; i < FRAME_TABLE_SIZE; i++) {
            if (frame_table[i].is_used == false) { // Find empty frame
                page_table[task2_value->pnumber].is_present = true; // Mark page as present

                frame_table[i].is_used = true; // Mark frame as used
                page_table[task2_value->pnumber].frame_number = i; // Assign frame number
                task2_value->fnumber = i; // Set frame number

                task2_value->paddress = (i << OFFSET_BITS) | task1_value.poffset; // Calculate physical address
                break;
            }
        }

        // If no empty frame is found, we need to evict a page (FIFO algorithm)
        if (task2_value->fnumber == -1) {
            run_task3(task1_value, task2_value, page_table, frame_table, page_queue, tlb);
        }

        if (task2_value->fnumber != -1) {
            enqueue(page_queue, task2_value->fnumber); // Add frame number to the queue
        }
    }

    return;
};