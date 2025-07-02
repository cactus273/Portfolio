/**
 * Program to evict pages if necessary using a FIFO algorithm.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "task2.h"
#include "task4.h"

void run_task3(task1_value_t task1_value, task2_value_t *task2_value,
    page_table_entry_t *page_table, frame_t *frame_table, page_queue_t *page_queue, tlb_t *tlb) {

    int evicted_frame = dequeue(page_queue);
    
    if (evicted_frame != -1) {
        // Find which page was using this frame and mark it as not present
        for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
            if (page_table[i].is_present && page_table[i].frame_number == evicted_frame) {
                page_table[i].is_present = false;
                printf("evicted-page=%d,freed-frame=%d\n", i, evicted_frame);

                flush_tlb_entry(tlb, i); // Remove the page from TLB, if any
                
                break;
            }
        }

        // Free the frame and reassign it
        frame_table[evicted_frame].is_used = true;
        page_table[task2_value->pnumber].is_present = true;
        page_table[task2_value->pnumber].frame_number = evicted_frame;

        task2_value->fnumber = evicted_frame;
        task2_value->paddress = (evicted_frame << OFFSET_BITS) | task1_value.poffset;
    }

    return;
}