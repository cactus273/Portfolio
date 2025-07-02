/*
    Written by Jesslyn Andriono
    on 23 May 2025
    to complete Stage 4
    for COMP30023 Assignment 2 2025 Semester 1

    Header file for stage4.c, which:
    Expires stale cached items based on the max age.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/

#ifndef STAGE4_H
#define STAGE4_H

#include <time.h>
#include "stage2.h"

// Returns 0 if no max-age, otherwise returns expiration timestamp
time_t get_cache_expiry(const char *response);

// Check if response is expired
int is_entry_stale(cache_entry_t *entry, char *target_host);

// Find a cache entry by request key
cache_entry_t *find_cache_entry(cache_t *cache, char *request);

// Evict a specific cache entry
void evict_cache_entry(cache_t *cache, cache_entry_t *entry, char *target_host);

/* This is used in the case of a stale entry, where the newly obtained
    response is able to be cached. */
void update_cache_entry(cache_t *cache, cache_entry_t *entry, char *response_value, size_t size);

#endif