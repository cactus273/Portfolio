/*
    Written by Bryan Susanto, Jesslyn Andriono
    on 12-14, 20-23 May 2025
    to generate the htproxy executable
    for COMP30023 Assignment 2 2025 Semester 1

    Make using
        make htproxy

    Run using e.g.
        ./htproxy -p 8080 [-c]

    Handles the command-line arguments and main loop
        for accepting requests and delegates tasks
        to other modules.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
    Socket programming code heavily borrowed
        from Beej's networking guide.
*/
#include "main.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "stage4.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 10

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IOLBF, 0);

    // Process the command-line arguments
    char *listen_port;
    int is_cache;
    get_arguments(argc, argv, &listen_port, &is_cache);

    // Setup listening half-socket
    int sockfd = setup_half_socket_for_listen(listen_port);

    // Listen on that half-socket
    listen(sockfd, BACKLOG);

    // Initialize cache
    cache_t *cache = NULL;
    if (is_cache) {
        cache = create_cache();
        assert(cache);
    }

    // Main request loop
    // Aside:
    // - "client" for computer using this as proxy
    // - "server" for the server they want to actually access
    int client_sockfd, server_sockfd, response_body_length;
    char *request = NULL;
    char *request_tail = NULL;
    char *target_host = NULL;
    char *request_URI = NULL;
    size_t sent_size = 0;

    while (1) {
        sent_size = 0;
        // accept() returns a new full socket for us ready to send(), recv()
        // and puts information about the client in the provided structs
        client_sockfd = accept(sockfd, NULL, NULL);
        if (client_sockfd == -1) {
            // socket creation with requesting client failed
            // so we just move on to the next request
            perror("accept");
            continue;
        }

        // successfully created the connection socket
        printf("Accepted\n");
        fflush(stdout);

        // handle getting the HTTP request
        if (parse_http_request(client_sockfd, &request, &target_host,
                               &request_tail) == -1) {
            // something went wrong in either recv'ing the request header or
            // parsing it
            free_parse_http_request(&request, &target_host, &request_tail);
            close(client_sockfd);
            continue;
        }
        printf("Request tail %s\n", request_tail);
        fflush(stdout);

        get_request_URI(request, &request_URI);

        // Check for cache hit and staleness
        cache_entry_t *entry = NULL;
        int was_stale = 0;

        if (strlen(request) < 2000 && is_cache && cache) {
            entry = find_cache_entry(cache, request);
            if (entry && is_entry_stale(entry, target_host)) {
                was_stale = 1;
            }
        }

        if (entry && !was_stale && serve_cache_hit(cache, request, client_sockfd, target_host, request_URI, &sent_size)) {
            fflush(stdout);
            free_parse_http_request(&request, &target_host, &request_tail);
            free_get_request_URI(&request_URI);
            close(client_sockfd);
            continue;
        }

        // EVICTION: If cache is full and we're about to fetch a new response, evict now
        if (is_cache && !was_stale && strlen(request) < 2000 && cache && cache->count >= MAX_CACHE_COUNT) {
            evict_lru_entry(cache, target_host);
        }

        // Only for cache misses: print GETting after eviction, before connecting
        if (was_stale || !entry) {
            free_get_request_URI(&request_URI);
            get_request_URI(request, &request_URI);
            printf("GETting %s %s\n", target_host, request_URI);
            fflush(stdout);
        }


        // Now make the connection to the server and send over the request
        server_sockfd = connect_to_server(target_host);
        if (server_sockfd == -1) {
            // failed to connect to server
            // so we just move on to the next request
            perror("server: connect");
            free_parse_http_request(&request, &target_host, &request_tail);
            free_get_request_URI(&request_URI);
            close(client_sockfd);
            continue;
        }

        if (send_request_to_host(server_sockfd, request, strlen(request)) ==
            -1) {
            // oops something went wrong in the sending process
            perror("send");
            free_parse_http_request(&request, &target_host, &request_tail);
            free_get_request_URI(&request_URI);
            close(client_sockfd);
            close(server_sockfd);
            continue;
        }

        // Prepare to receive the response, for caching
        char *response_buffer = NULL;
        int response_size = 0;
        int is_not_cacheable = 0;

        // And now send the response back as we go, optionally caching it
        if (is_cache && strlen(request) < 2000 && cache) {
            int content_length = 0;
            response_size = process_and_send_response_with_buffer(
                server_sockfd, client_sockfd, &response_buffer, &content_length);
            if (response_size == -1) {
                // something went wrong in reading/sending stuff over
                perror("send");
                free_parse_http_request(&request, &target_host, &request_tail);
                free_get_request_URI(&request_URI);
                close(client_sockfd);
                close(server_sockfd);
                continue;
            }
            // content_length is set by the function to the body length

            if (response_size > 0 && response_size <= 102400 
                && is_cacheable(response_buffer)) {
                    if (was_stale) {
                        // `entry` is the stale entry to be updated
                        // if was_stale was set to 1.
                        update_cache_entry(cache, entry, response_buffer, response_size);
                    } else {
                        add_cache_entry(cache, request, response_buffer, response_size, target_host);
                    }
            } else if (!is_cacheable(response_buffer)) {
                is_not_cacheable = 1;
                response_body_length = content_length;
                
                // If not cacheable, print after logging GET and response body length
                printf("Response body length %d\n", response_body_length); // response_body_length should be set to the actual bytes sent
                fflush(stdout);
                printf("Not caching %s %s\n", target_host, request_URI);
                fflush(stdout);

                // And if this was related to an entry that has gone stale,
                // we must evict it
                if (was_stale) {
                    evict_cache_entry(cache, entry, target_host);
                    entry = NULL;
                }
            }

            if (response_buffer) free(response_buffer);

            response_body_length = content_length;
        } else {
            if ((response_body_length = process_and_send_response(
                    server_sockfd, client_sockfd)) == -1) {
                // again, something went wrong in reading/sending stuff over
                perror("send");
                free_parse_http_request(&request, &target_host, &request_tail);
                free_get_request_URI(&request_URI);
                close(client_sockfd);
                close(server_sockfd);
                continue;
            }
        }
        if (!is_not_cacheable) {
            // After sending the response (from server to client), always print:
            printf("Response body length %d\n", response_body_length); // response_body_length should be set to the actual bytes sent
            fflush(stdout);
        }
        // Eviction message after
        if (was_stale && (response_size < 0 || response_size > 102400)) {
            // the new response is too big
            // thus can't replace the old cache
            // so we need to evict the stale cache
            evict_cache_entry(cache, entry, target_host);
            entry = NULL;
        }

        // Clean-up
        free_parse_http_request(&request, &target_host, &request_tail);
        free_get_request_URI(&request_URI);
        close(client_sockfd);
        close(server_sockfd);
    }

    // Clean-up just in case
    if (cache) free_cache(cache);
    return 0;
}

/* Parses the command-line arguments with flags in any order,
    and sets the passed in pointers' values based on the
    command-line arguments */
void get_arguments(int argc, char **argv, char **listen_port, int *is_cache) {
    *is_cache = 0;
    if (argc < 3) {
        fprintf(stderr, "Too little arguments! Usage: ./htproxy -p listen-port [-c]\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            *listen_port = argv[++i];
        } else if (strcmp(argv[i], "-c") == 0) {
            *is_cache = 1;
        }
    }
}