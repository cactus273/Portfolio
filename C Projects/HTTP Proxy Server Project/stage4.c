/*
    Written by Jesslyn Andriono, Bryan Susanto
    on 23-24 May 2025
    to complete Stage 4
    for COMP30023 Assignment 2 2025 Semester 1

    Expires stale cached items based on the max age.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/

#define _GNU_SOURCE

#include "stage4.h"
#include "stage3.h"
#include "stage2.h"
#include "stage1.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Returns 0 if no max-age, otherwise returns expiration timestamp
time_t get_cache_expiry(const char *response) {
    if (!response) return 0;
    
    const char *header_end = strstr(response, "\r\n\r\n");
    if (!header_end) return 0;

    // Calculate length of headers
    size_t header_len = header_end - response;
    char *headers = malloc(header_len + 1);
    if (!headers) return 0;
    strncpy(headers, response, header_len);
    headers[header_len] = '\0';

    // Case-insensitive search for Cache-Control
    const char *header = strcasestr(headers, "Cache-Control:");
    time_t expiry = 0;

    if (header) {
        header += strlen("Cache-Control:");
        
        // Skip whitespace
        while (*header == ' ' || *header == '\t') header++;
        
        const char *eol = strchr(header, '\r');
        if (!eol) eol = strchr(header, '\n');
        size_t len = eol ? (size_t)(eol - header) : strlen(header);

        char *buf = malloc((len + 1) * sizeof(*buf));
        assert(buf);
        strncpy(buf, header, len);
        buf[len] = '\0';
        
        // Convert to lowercase for case-insensitive matching
        for (char *p = buf; *p; ++p) *p = tolower(*p);

        // Look for max-age
        char *maxage = strstr(buf, "max-age=");
        if (maxage) {
            int seconds = atoi(maxage + strlen("max-age="));
            if (seconds > 0) {
                expiry = time(NULL) + seconds;
            }
        }

        free(buf);
    }

    free(headers);
    return expiry;
}

// Check if response is expired
int is_entry_stale(cache_entry_t *entry, char *target_host) {
    if (!entry || !target_host) return 0;
    
    // If no expiry is set, entry doesn't get stale
    if (entry->expiry == 0) return 0;

    time_t now = time(NULL);
    if (now >= entry->expiry) {
        // Entry is stale 
        char *request_URI = NULL;
        get_request_URI(entry->request_key, &request_URI);
        
        printf("Stale entry for %s %s\n", target_host, request_URI ? request_URI : "");
        fflush(stdout);
        
        if (request_URI) {
            free_get_request_URI(&request_URI);
        }
        return 1; // Stale
    }
    return 0; // Not stale
}

// Find a cache entry by request key
cache_entry_t *find_cache_entry(cache_t *cache, char *request) {
    if (!cache || !request) return NULL;
    
    for (int i = 0; i < cache->count; i++) {
        if (cache->entry[i] && cache->entry[i]->request_key && 
            strcmp(cache->entry[i]->request_key, request) == 0) {
            return cache->entry[i];
        }
    }
    return NULL; // Not found
}

// Evict a specific cache entry
void evict_cache_entry(cache_t *cache, cache_entry_t *entry, char *target_host) {
    if (!cache || !entry) return;
    
    for (int i = 0; i < cache->count; i++) {
        if (cache->entry[i] == entry) {
            // Extract request URI from the evicted entry's request_key
            char *request_URI = NULL;
            get_request_URI(cache->entry[i]->request_key, &request_URI);
            printf("Evicting %s %s from cache\n", target_host, request_URI ? request_URI : "");
            fflush(stdout);

            if (request_URI) free_get_request_URI(&request_URI);

            // Free the memory
            if (cache->entry[i]->request_key) {
                free(cache->entry[i]->request_key);
            }
            if (cache->entry[i]->response_value) {
                free(cache->entry[i]->response_value);
            }
            free(cache->entry[i]);
            
            // Move last entry to this position to fill the gap
            cache->entry[i] = cache->entry[cache->count - 1];
            cache->count--;
            break;
        }
    }
}

/* This is used in the case of a stale entry, where the newly obtained
    response is able to be cached. */
void update_cache_entry(cache_t *cache, cache_entry_t *entry, char *response_value, size_t size) {
    if (entry->response_value) free(entry->response_value);
    entry->response_value = malloc(size);
    assert(entry->response_value);
    memcpy(entry->response_value, response_value, size);
    entry->size = size;
    entry->last_used_time = cache->time++;
    entry->expiry = get_cache_expiry(response_value);
}