/*
    Written by Jesslyn Andriono
    on 20 May 2025
    to complete Stage 2
    for COMP30023 Assignment 2 2025 Semester 1

    Contains functions for the naive caching to store
        requests and responses.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/

#include "stage1.h"
#include "stage2.h"
#include "stage4.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Initialize the cache
cache_t *create_cache() {
    cache_t *cache = malloc(sizeof(cache_t));
    assert(cache);
    cache->count = 0;
    cache->time = 0;
    for (int i = 0; i < MAX_CACHE_COUNT; i++) {
        cache->entry[i] = NULL;
    }

    return cache;
}

// Create new cache entry
cache_entry_t *create_cache_entry(char *request_key, char *response_value, size_t size) {
    cache_entry_t *entry = malloc(sizeof(cache_entry_t));
    assert(entry);
    entry->request_key = strdup(request_key);
    entry->response_value = malloc(size);
    assert(entry->response_value);
    memcpy(entry->response_value, response_value, size);
    entry->size = size;
    entry->last_used_time = 0;
    entry->expiry = 0;
    return entry;
}

// Free the cache
void free_cache(cache_t *cache) {
    for (int i = 0; i < cache->count; i++) {
        free(cache->entry[i]->request_key);
        free(cache->entry[i]->response_value);
        free(cache->entry[i]);
    }
    free(cache);
}

// Lookup a request in the cache by key value
int check_cache(cache_t *cache, char *request_key, char **response_value, size_t *response_size) {
    for (int i = 0; i < cache->count; i++) {
        if (strcmp(cache->entry[i]->request_key, request_key) == 0) {
            *response_value = cache->entry[i]->response_value;
            cache->entry[i]->last_used_time = cache->time++;
            *response_size = cache->entry[i]->size;     
            return 1; // Key match in cache
        }
    }
    return 0; // No matching key in cache
}

// Add a new entry to the cache
int add_cache_entry(cache_t *cache, char *request_key, char *response_value, 
                    size_t size, char *target_host) {
    // Check if cache is full
    if (cache->count >= MAX_CACHE_COUNT) {
        // Evict the least recently used entry using the helper
        evict_lru_entry(cache, target_host);
    }
    // Create new cache entry
    cache_entry_t *new_entry = create_cache_entry(request_key, response_value, size);
    assert(new_entry);
    new_entry->last_used_time = cache->time++;
    new_entry->expiry = get_cache_expiry(response_value);
    cache->entry[cache->count++] = new_entry;
    return 0;
}

// Evict the least recently used entry from the cache
void evict_lru_entry(cache_t *cache, char *target_host) {
    if (cache->count == 0) return;
    int lru_index = 0;
    for (int i = 1; i < cache->count; i++) {
        if (cache->entry[i]->last_used_time < cache->entry[lru_index]->last_used_time) {
            lru_index = i;
        }
    }

    // Extract request URI from the evicted entry's request_key
    char *request_URI = NULL;
    get_request_URI(cache->entry[lru_index]->request_key, &request_URI);
    printf("Evicting %s %s from cache\n", target_host, request_URI ? request_URI : "");
    fflush(stdout);

    if (request_URI) free_get_request_URI(&request_URI);

    free(cache->entry[lru_index]->request_key);
    free(cache->entry[lru_index]->response_value);
    free(cache->entry[lru_index]);

    // Move last entry to fill the gap
    cache->entry[lru_index] = cache->entry[cache->count - 1];
    cache->entry[cache->count - 1] = NULL;
    cache->count--;
}

// Serve a cache hit 
int serve_cache_hit(
    cache_t *cache,
    char *request,
    int client_sockfd,
    char *target_host,
    char *request_URI,
    size_t *sent_size
) {
    char *cached_response = NULL;
    size_t cached_response_size = 0;
    if (check_cache(cache, request, &cached_response, &cached_response_size) && cached_response) {
        ssize_t total_sent = 0;
        while (total_sent < (ssize_t)cached_response_size) {
            ssize_t sent = send(client_sockfd, cached_response + total_sent, cached_response_size - total_sent, 0);
            if (sent < 0) break;
            total_sent += sent;
        }
        printf("Serving %s %s from cache\n", target_host, request_URI);
        fflush(stdout);
        if (sent_size) *sent_size = cached_response_size;
        return 1; // Cache hit and served
    }
    return 0; // Not in cache
}
