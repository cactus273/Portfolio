/*
    Written by Jesslyn Andriono
    on 23 May 2025
    to complete Stage 3
    for COMP30023 Assignment 2 2025 Semester 1

    Header file for stage3.c, which:
    Contains functions for the valid caching to store
        requests and responses.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/
#ifndef STAGE3_H
#define STAGE3_H

#include <stddef.h>

int is_cacheable(const char *response);

#endif