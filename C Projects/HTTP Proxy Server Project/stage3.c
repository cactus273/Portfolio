/*
    Written by Jesslyn Andriono
    on 23 May 2025
    to complete Stage 3
    for COMP30023 Assignment 2 2025 Semester 1

    Contains functions for the valid caching to store
        requests and responses.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/

#define _GNU_SOURCE

#include "stage2.h"
#include "stage1.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Check if the response is cacheable (cache-control header)
int is_cacheable(const char *response) {
    const char *header_end = strstr(response, "\r\n\r\n");
    if (!header_end) return 1; // No headers, cacheable

    size_t header_len = header_end - response;
    char *headers = malloc(header_len + 1);
    if (!headers) return 1; // Fail open (cacheable) on allocation error
    strncpy(headers, response, header_len);
    headers[header_len] = '\0';

    // Case-insensitive search for Cache-Control
    const char *header = strcasestr(headers, "Cache-Control:");
    int result = 1; // Default is cacheable

    if (header) {
        // Move past "Cache-Control:"
        header += strlen("Cache-Control:");
        // Find end of line
        const char *eol = strchr(header, '\n');
        size_t len = eol ? (size_t)(eol - header) : strlen(header);

        // char buf[CACHE_CONTROL_BUF_SIZE] = {0};
        // strncpy(buf, header, len > 255 ? 255 : len);

        char *buf = malloc((len + 1) * sizeof(*buf));
        assert(buf);
        strncpy(buf, header, len);
        buf[len] = '\0';

        for (char *p = buf; *p; ++p) *p = tolower(*p);

        if (strstr(buf, "private") ||
            strstr(buf, "no-store") ||
            strstr(buf, "no-cache") ||
            strstr(buf, "max-age=0") ||
            strstr(buf, "must-revalidate") ||
            strstr(buf, "proxy-revalidate")) {
            result = 0; // Not cacheable
        }

        free(buf);
    }

    free(headers);
    return result;
}