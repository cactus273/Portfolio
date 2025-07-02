/*
    Written by Bryan Susanto, ...
    on 12-14, ... May 2025
    to generate the htproxy executable
    for COMP30023 Assignment 2 2025 Semester 1

    Header file for main.c, which:
    Handles the command-line arguments and main loop
        for accepting requests and delegates tasks
        to other modules.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/
#ifndef MAIN_H
#define MAIN_H

/* Parses the command-line arguments with flags in any order,
    and sets the passed in pointers' values based on the
    command-line arguments */
void get_arguments(int argc, char **argv, char **listen_port, int *cache);

#endif