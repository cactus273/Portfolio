/**
 * Program to process command line arguments and run tasks based on user input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "task1.h"
#include "task2.h"
#include "task4.h"
#include "main.h"

/* TYPE DEFINITIONS ----------------------------------------------------------*/
page_table_entry_t page_table[PAGE_TABLE_SIZE];    // Page table with 1024 entries
frame_t frame_table[FRAME_TABLE_SIZE];             // Frame table with 256 entries
 
/* LINKED LIST FUNCTIONS -----------------------------------------------------*/
void add_node(list_t *list, int memory_address) {
    node_t *new_node = malloc(sizeof(node_t));
    assert(new_node);
    
    new_node->memory_address = malloc(sizeof(int));
    assert(new_node->memory_address);

    *(new_node->memory_address) = memory_address;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
        list->foot = new_node;
    } else {
        list->foot->next = new_node;
        list->foot = new_node;
    }
}
 
void free_list(list_t *list) {
    node_t *current = list->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current->memory_address);
        free(current);
        current = next;
    }
    free(list);
}
 
list_t *create_list() {
    list_t *list = malloc(sizeof(list_t));
    assert(list);
    list->head = NULL;
    list->foot = NULL;
    return list;
}

void remove_node(list_t *list, int memory_address) {
    if (list->head == NULL) {
        return; // List is empty
    }

    node_t *current = list->head;
    node_t *previous = NULL;
 
    while (current != NULL) {
        if (*(current->memory_address) == memory_address) {
            if (previous == NULL) {
                // Removing the head node
                list->head = current->next;
                if (list->head == NULL) {
                    list->foot = NULL; // List is now empty
                }
            } else {
                // Removing a middle or tail node
                previous->next = current->next;
                if (current == list->foot) {
                    list->foot = previous; // Update foot if tail node is removed
                }
            }
            free(current->memory_address);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

 /* PAGE QUEUE FUNCTIONS ------------------------------------------------------*/
page_queue_t *createQueue(int capacity) {
    page_queue_t *queue = (page_queue_t*)malloc(sizeof(page_queue_t));
    assert(queue);

    if (!queue) return NULL;
    
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->frame_numbers = (int*)malloc(queue->capacity * sizeof(int));
    assert(queue->frame_numbers);
    
    if (!queue->frame_numbers) {
        free(queue);
        return NULL;
    }
    
    return queue;
}

void destroyQueue(page_queue_t* queue) {
    if (queue) {
        if (queue->frame_numbers) {
            free(queue->frame_numbers);
        }
        free(queue);
    }
}

void enqueue(page_queue_t* queue, int frame_number) {
    if (queue->size == queue->capacity)
        return;
        
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->frame_numbers[queue->rear] = frame_number;
    queue->size = queue->size + 1;
}

int dequeue(page_queue_t* queue) {
    if (queue->size == 0)
        return -1;
        
    int frame_number = queue->frame_numbers[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return frame_number;
}
 
/* PAGE TABLE, FRAME TABLE, AND TLB FUNCTIONS ---------------------------------*/
page_table_entry_t* create_page_table() {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        page_table[i].is_present = false; // Initialize all entries as not present
        page_table[i].frame_number = -1; // Invalid frame number
    }

    return page_table;
}

frame_t* create_frame_table() {
    for (int i = 0; i < FRAME_TABLE_SIZE; i++) {
        frame_table[i].is_used = false; // Initialize frame data
    }

    return frame_table;
}

tlb_t* create_tlb() {
    tlb_t *tlb = malloc(sizeof(tlb_t));
    assert(tlb);

    tlb->size = 0; // Initialize size to 0
    tlb->time = 0; // Reset time

    for (int i = 0; i < TLB_SIZE; i++) {
        tlb->entries[i].page_number = -2; // Initialize entries as invalid
        tlb->entries[i].frame_number = -2; // Invalid frame number
    }

    return tlb;
}

tlb_t* destroy_tlb(tlb_t *tlb) {
    if (tlb) {
        free(tlb);
    }
    return NULL;
}
 
/* TASK FUNCTIONS ------------------------------------------------------------*/
list_t *store_memory_accesses(const char *input_filename) {
    char full_path[FILENAME_MAX];
    snprintf(full_path, FILENAME_MAX, "%s", input_filename);

    // Check if the file exists
    FILE *file = fopen(full_path, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Store integer values from file in a linked list
    list_t *memory_accesses = create_list();
    if (!memory_accesses) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read addresses from the file and add them to the linked list
    int count = 0;
    while (!feof(file)) {
        int memory_address;
        if (fscanf(file, "%d", &memory_address) == 1) {
            add_node(memory_accesses, memory_address);
            count++;
        }
    }

    fclose(file);
    return memory_accesses;
}
 
void free_memory_accesses(list_t *memory_accesses) {
    if (memory_accesses) {
        free_list(memory_accesses);
    }
}
 
// -t is either task1, task2, task3, or task4
int is_valid_task(const char *task) {
    return strcmp(task, "task1") == 0 || strcmp(task, "task2") == 0 ||
        strcmp(task, "task3") == 0 || strcmp(task, "task4") == 0;
}
 
/* MAIN FUNCTION --------------------------------------------------------------*/
/* Main function to process command line arguments and run tasks based on user input */
int main(int argc, char *argv[]) {
    char *input_filename = NULL;
    char *task = NULL;
 
    // Check if the correct number of arguments are provided
    if (argc < 5) {
        fprintf(stderr, "Usage: %s -f <filename> -t <task>\n", argv[0]);
        return 1;
    }

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            input_filename = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            task = argv[++i];
        }
    }

    // Check if input filename and task are provided
    if (!input_filename || !task) {
        fprintf(stderr, "Usage: %s -f <filename> -t <task>\n", argv[0]);
        return 1;
    }
 
    if (!is_valid_task(task)) {
        fprintf(stderr, "Invalid task. Allowed tasks are: task1, task2, task3, task4.\n");
        return 1;
    }

    list_t *memory_accesses = store_memory_accesses(input_filename);
    if (!memory_accesses) {
        fprintf(stderr, "Failed to store memory accesses.\n");
        return 1;
    }

    // Create page table, frame table, and TLB
    page_table_entry_t *page_table = create_page_table();
    frame_t *frame_table = create_frame_table();
    tlb_t *tlb = create_tlb();

    // Create a page queue for FIFO page replacement
    page_queue_t *page_queue = createQueue(FRAME_TABLE_SIZE);

    task1_value_t task1_value = {-1, -1, -1};
    task2_value_t task2_value = {-1, false, -1, -1};
 
    // Traverse through the memory accesses and run the specified task
    node_t *current = memory_accesses->head;
    while (current != NULL) {
        if (strcmp(task, "task1") == 0) {
            // Run task1: print the logical address, page number, and offset
            run_task1(*(current->memory_address), &task1_value);
            printf("logical-address=%d,page-number=%d,offset=%d\n",
                task1_value.laddress, task1_value.pnumber, task1_value.poffset);
        } else if (strcmp(task, "task2") == 0 || strcmp(task, "task3") == 0) {
            // Run task1
            run_task1(*(current->memory_address), &task1_value);
            printf("logical-address=%d,page-number=%d,offset=%d\n",
                task1_value.laddress, task1_value.pnumber, task1_value.poffset);

            // then run task2: print the page number, page fault, frame number, and physical address
            // if frame needs evicting, run task3
            run_task2(task1_value, &task2_value, page_table, frame_table, page_queue, tlb);
            printf ("page-number=%d,page-fault=%d,frame-number=%d,physical-address=%d\n",
                task2_value.pnumber, task2_value.pfault, task2_value.fnumber, task2_value.paddress);
        } else if (strcmp(task, "task4") == 0) {
             // Run task1
             run_task1(*(current->memory_address), &task1_value);
             printf("logical-address=%d,page-number=%d,offset=%d\n",
                 task1_value.laddress, task1_value.pnumber, task1_value.poffset);

            // Run task4: check if page number is in TLB
            // If not in TLB, run task2: print the page number, page fault, frame number, and physical address
            // if frame needs evicting, run task3
            run_task4(task1_value, &task2_value, tlb, page_table, frame_table, page_queue);
        } else {
            return 1;
        }
        current = current->next;
    }

    free_memory_accesses(memory_accesses);
    destroyQueue(page_queue);
    destroy_tlb(tlb);

    return 0;
}