/*
    Written by Jesslyn Andriono
    on 20 May 2025
    to complete Stage 2
    for COMP30023 Assignment 2 2025 Semester 1

    Header file for stage2.c, which:
    Contains functions for the naive caching to store
        requests and responses.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/
#ifndef STAGE2_H
#define STAGE2_H

#include <stddef.h>
#include <time.h>

#define MAX_ENTRY_SIZE 102400   // 100 KiB
#define MAX_CACHE_COUNT 10      // 10 cache entries
// #define MAX_CACHED_REQUEST 2000 // 2000 bytes

/* TYPE DEFINITIONS ----------------------------------------------------------*/
// Cache entry definition
typedef struct {
    char* request_key;                      // HTTP request
    char* response_value;                   // HTTP response
    size_t size;                            // HTTP response size (≤ 100 KiB)
    int last_used_time;                     // Timestamp for LRU eviction
    time_t expiry;                          // Expiry time for cache entry
} cache_entry_t;

// Cache definition
typedef struct {
    cache_entry_t* entry[MAX_CACHE_COUNT];  // Pointer to cache_entry_t array
    int count;                              // Number of cache entries
    int time;                               // Time for LRU eviction
} cache_t;

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
// Initialize the cache
cache_t *create_cache();

// Create new cache entry
cache_entry_t *create_cache_entry(char *request_key, char *response_value, size_t size);

// Free the cache
void free_cache(cache_t *cache);

// Lookup a request in the cache by key value
int check_cache(cache_t *cache, char *request_key, char **response_value, size_t *response_size);

// Add a new entry to the cache
int add_cache_entry(cache_t *cache, char *request_key, char *response_value, 
                    size_t size, char *target_host);

// Evict the least recently used entry from the cache
void evict_lru_entry(cache_t *cache, char *target_host);

int serve_cache_hit(
    cache_t *cache,
    char *request,
    int client_sockfd,
    char *target_host,
    char *request_URI,
    size_t *sent_size
);

#endif