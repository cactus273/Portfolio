#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARSED_BITS 22                  // Total bits for logical address
#define PAGE_NUMBER_BITS 10             // Number of bits for page number
#define OFFSET_BITS 12                  // Number of bits for offset

#define PAGE_TABLE_SIZE 1024            // Number of pages in the page table
#define FRAME_TABLE_SIZE 256            // Number of frames in the frame table
#define TLB_SIZE 32                    // Number of entries in the TLB

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct node node_t; // A node in a linked list

struct node {
    int* memory_address;    // The memory address accessed
    node_t* next;           // Next node in the list
};

typedef struct {
    int count;              // Number of nodes in the list
    node_t* head;           // Pointer to the first node
    node_t* foot;           // Pointer to the last node
} list_t;

typedef struct {
    _Bool is_present;       // 1 if the page is mapped to a frame, 0 otherwise
    int frame_number;       // Frame number where the page is stored
} page_table_entry_t;

typedef struct {
    _Bool is_used;          // 1 if the frame is in use, 0 otherwise
} frame_t;

typedef struct {
    int front, rear, size;  // Index of front and rear, size of the queue
    unsigned capacity;      // Maximum number of frames in the queue
    int* frame_numbers;     // Array to store frame numbers
} page_queue_t; 

typedef struct {
    int page_number;
    int frame_number;     
    int last_used_time;     // Time of last access for LRU  
} tlb_entry_t;

typedef struct {
    int size;                       // Size of the TLB
    int time;                       // Global tracking variable for LRU
    tlb_entry_t entries[TLB_SIZE];  // Array of TLB entries
} tlb_t;


/* FUNCTION PROTOTYPES -------------------------------------------------------*/
list_t* create_list();
void add_node(list_t* list, int memory_address);
void remove_node(list_t* list, int memory_address);
void free_list(list_t* list);
page_queue_t* createQueue(int capacity);
void destroyQueue(page_queue_t* queue);
void enqueue(page_queue_t* queue, int frame_number);
int dequeue(page_queue_t* queue);
list_t* store_memory_accesses(const char* input_filename);
int is_valid_task(const char* task);

#endif