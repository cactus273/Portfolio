/*
    Written by Bryan Susanto
    on 12-15 May 2025
    to complete Stage 1
    for COMP30023 Assignment 2 2025 Semester 1

    Header file for stage1.c, which:
    Contains functions used primarily in stage 1,
        which is to implement a simple HTTP web proxy.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/
#ifndef STAGE1_H
#define STAGE1_H

/* Sets up a half socket locally on the given listen_port that is
    ready to be listen()'d and returns the file descriptor */
int setup_half_socket_for_listen(char *listen_port);

/* Reads in HTTP request from given socket file descriptor
    and stores the request in **request as well as parses the
    target host and stores it in **target_host. Returns -1
    when there is an error in parsing. */
int parse_http_request(int client_sockfd, char **request, char **target_host,
                       char **request_tail);
void free_parse_http_request(char **request, char **target_host,
                             char **request_tail);

/* Finds the *header_name header within *http_header, and
    writes its value in *header_value, including allocating
    enough memory for it. Assumes *header_name includes the ": ".
    Returns -1 if unable to find header. */
int find_header(char *http_header, char *header_name, char **header_value);
void free_find_header(char **header_value);

/* Sets up a connection with the given target_host string
    and returns the file descriptor. Returns -1 on error. */
int connect_to_server(char *target_host);

/* Sends over the string to the socket fd */
int send_request_to_host(int sockfd, char *buf, int num_bytes);

/* Takes in response from the server socket fd, and sends that over
    piece by piece to the client socket fd after the response header
    is parsed. */
int process_and_send_response(int server_sockfd, int client_sockfd);
int process_and_send_response_with_buffer(int server_sockfd, int client_sockfd, 
                                            char **response_buffer, int *content_length_out);

/* Gets the request-URI from the request and sets the value
    of *request_URI from that. */
void get_request_URI(char *request, char **request_URI);
void free_get_request_URI(char **request_URI);

/* Searches n characters of the string *haystack for
    the substring *needle, and returns a pointer to
    the start of the found *needle, and NULL otherwise.
    Only assumes that needle is a proper null-terminated string; doesn't assume that haystack is properly null-terminated.
    Naming convention inspired by man7.org strstr(3). */
char *search_n_substr(char *haystack, char *needle, int haystack_len);

#endif