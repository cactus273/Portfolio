/**
 * Program to implement translation lookaside buffer (TLB).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "task1.h"
#include "task2.h"

// Helper function to create a TLB entry
void add_tlb_entry(tlb_t *tlb, int entry_index, int page_number, int frame_number) {
    tlb->entries[entry_index].page_number = page_number;
    tlb->entries[entry_index].frame_number = frame_number;
    tlb->entries[entry_index].last_used_time = tlb->time++; // Set last used time to current time + 1
    // the higher the value the more recently used

    return;
}

void run_task4(task1_value_t task1_value, task2_value_t *task2_value, tlb_t *tlb, 
    page_table_entry_t *page_table, frame_t *frame_table, page_queue_t *page_queue) {
    _Bool tlb_hit = false;
    
    // Check if page number is in TLB
    for (int i = 0; i < tlb->size; i++) {
        if (tlb->entries[i].page_number == task1_value.pnumber) {
            tlb_hit = true;
            task2_value->fnumber = tlb->entries[i].frame_number; // Get frame number
            task2_value->paddress = (task2_value->fnumber << OFFSET_BITS) | task1_value.poffset; // Calculate physical address
            task2_value->pfault = false;
            
            // Mark as most recently used
            tlb->entries[i].last_used_time = tlb->time++;

            printf("tlb-hit=%d,page-number=%d,frame=%d,physical-address=%d\n",
                tlb_hit, task1_value.pnumber, task2_value->fnumber, task2_value->paddress);
            
            return;
        }
    }
    
    // Print the TLB miss information
    printf("tlb-hit=%d,page-number=%d,frame=none,physical-address=none\n",
        tlb_hit, task1_value.pnumber);
    
    // Run task2 to get the frame number and page fault status
    run_task2(task1_value, task2_value, page_table, frame_table, page_queue, tlb);

    // Update TLB with the information
    if (tlb->size < TLB_SIZE) {
        // TLB has empty space, add new entry
        add_tlb_entry(tlb, tlb->size, task1_value.pnumber, task2_value->fnumber);
        tlb->size++;

        printf("tlb-remove=none,tlb-add=%d\n", task1_value.pnumber);
    } else {
        // TLB is full, evict least recently used entry
        int lru_entry_index = 0;
        int min_timestamp = tlb->entries[lru_entry_index].last_used_time; // Initialize with the first entry's timestamp
        
        // Find the least recently used entry by finding the minimum timestamp
        for (int i = 1; i < TLB_SIZE; i++) {
            if (tlb->entries[i].last_used_time < min_timestamp) {
                min_timestamp = tlb->entries[i].last_used_time;
                lru_entry_index = i;
            }
        }
        
        int removed_page = tlb->entries[lru_entry_index].page_number;
        
        // Replace the LRU entry with the new entry at the same index
        add_tlb_entry(tlb, lru_entry_index, task1_value.pnumber, task2_value->fnumber);

        printf("tlb-remove=%d,tlb-add=%d\n", removed_page, task1_value.pnumber);
    }

    printf ("page-number=%d,page-fault=%d,frame-number=%d,physical-address=%d\n",
        task2_value->pnumber, task2_value->pfault, task2_value->fnumber, task2_value->paddress);

    return;
}

// Flushes an entry from the TLB when a page is evicted from memory
void flush_tlb_entry(tlb_t *tlb, int page_number) {
    // Check if page number is in TLB
    for (int i = 0; i < tlb->size; i++) {
        if (tlb->entries[i].page_number == page_number) {
            // Shift remaining entries up to remove entry
            for (int j = i; j < tlb->size - 1; j++) {
                tlb->entries[j] = tlb->entries[j + 1];
            }
            tlb->size--;

            printf("tlb-flush=%d,tlb-size=%d\n", page_number, tlb->size);
            break;
        }
    }

    return;
}