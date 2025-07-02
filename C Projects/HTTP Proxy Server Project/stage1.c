/*
    Written by Bryan Susanto
    on 12-15, 24- May 2025
    to complete Stage 1
    for COMP30023 Assignment 2 2025 Semester 1

    Contains functions used primarily in stage 1,
        which is to implement a simple HTTP web proxy.

    CREDITS:
    Authorship headers adapted from COMP20007
        assignment templates.
*/
#include "stage1.h"

#define _GNU_SOURCE // as required by the man pages for strstr()

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define REMOTE_HOST_PORT "80"
#define BUFFER_SIZE (1 << 7)

#define INITIAL_HEADER_SIZE (1 << 7)
#define INITIAL_TARGET_HOST_SIZE (1 << 4)

/* Sets up a half socket locally on the given listen_port that is
    ready to be listen()'d and returns the file descriptor */
int setup_half_socket_for_listen(char *listen_port) {
    int status, sockfd;
    // ---
    // STEP 1: Prepare required address information
    struct addrinfo hints, *results;
    // 1.1 Prepare hints to indicate
    memset(&hints, 0, sizeof hints); // as required by addrinfo struct
    hints.ai_family =
        AF_INET6; // use IPv6 which should also be compatible with IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP sockets
    hints.ai_flags = AI_PASSIVE;     // fill in local IP
    // 1.2 Get local address information
    if ((status = getaddrinfo(NULL, listen_port, &hints, &results)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    // ---
    // STEP 2: Loop through results and bind to the first possible one
    // (note that addrinfo gives a linked list back)
    struct addrinfo *p;
    for (p = results; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1) {
            // whoops we can't use that one
            // so look for the next possible one
            perror("proxy: socket");
            continue;
        }

        // yay we can use it (naive me)!
        // but perhaps someone else previously bound it
        // so we can't use it before the timeout is over
        // therefore as instructed we will override this behaviour
        int enable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
            0) {
            perror("setsockopt");
            exit(1);
        }

        // okay now we can bind it for real (hopefully)
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            // oops never mind, time to clean up that bind
            // and find another one that could perhaps work
            close(sockfd);
            perror("proxy: bind");
            continue;
        }

        break; // successfully bound a socket!
    }

    freeaddrinfo(results); // don't need them anymore

    if (p == NULL) {
        // means we've failed to bind a socket
        // as we didn't break early
        // and went through the whole list
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    return sockfd;
}

/* Reads in HTTP request from given socket file descriptor
    and stores the request in **request as well as parses the
    target host and stores it in **target_host. Returns -1
    when there is an error in parsing. */
int parse_http_request(int client_sockfd, char **request, char **target_host,
                       char **request_tail) {
    // PART 1: FINDING *request
    // Prepare an empty buffer
    char buf[BUFFER_SIZE];
    int buf_numbytes = 0;

    // Prepare request and target_host memory
    // Note: counting the number of bytes for request includes the '\0'
    size_t request_max_size = INITIAL_HEADER_SIZE;
    size_t request_content_i = 0;
    *request = malloc(request_max_size * sizeof(**request));
    assert(*request);

    // Main loop; keep going until we find the empty line
    char *terminator = NULL;
    while ((terminator = search_n_substr(*request, "\r\n\r\n", request_content_i)) == NULL) {
        // Read in as much as possible
        if ((buf_numbytes = recv(client_sockfd, buf, BUFFER_SIZE, 0)) ==
            -1 || buf_numbytes == 0) {
            perror("recv");
            return -1;
        }

        // Expand size of *request if needed.
        if (request_content_i + buf_numbytes > request_max_size) {
            request_max_size *= 2;
            *request = realloc(*request, request_max_size * sizeof(**request));
            assert(*request);
        }

        // Now that we have enough space, concat them.
        memcpy(*request + request_content_i, buf, buf_numbytes);
        request_content_i += buf_numbytes;
    }
    // Now that we're done, add the null terminating byte.
    if (request_max_size == request_content_i) {
        request_max_size *= 2;
        *request = realloc(*request, request_max_size * sizeof(**request));
        assert(*request);
    }
    (*request)[request_content_i] = '\0';

    // PART 2: FINDING *target_host
    if (find_header(*request, "Host: ", target_host) == -1) {
        fprintf(stderr, "Invalid host header in HTTP request\n");
        free(*request);
        *request = NULL;
        return -1;
    }

    // PART 3: FINDING *request_tail
    // Idea: Go backwards from end of *request until found "\r\n"
    char *curr =
        *request + strlen(*request) - strlen("\r\n\r\n") - strlen("\r\n");
    int last_line_len = 0;
    while (strncmp(curr, "\r\n", strlen("\r\n")) != 0) {
        curr--;
        last_line_len++;
    }
    // Now once we found, copy over to *request_tail
    // + 1 for the '\0'
    *request_tail = malloc((last_line_len + 1) * sizeof(**request_tail));
    assert(*request_tail);
    *request_tail =
        strncpy(*request_tail, curr + strlen("\r\n"), last_line_len);
    (*request_tail)[last_line_len] = '\0';

    return 0;
}
void free_parse_http_request(char **request, char **target_host,
                             char **request_tail) {
    free(*request);
    *request = NULL;
    // might not be allocated if parsing error
    if (*target_host) {
        free_find_header(target_host);
    }
    if (*request_tail) {
        free(*request_tail);
        *request_tail = NULL;
    }
}

/* Finds the *header_name header within *http_header, and
    writes its value in *header_value, including allocating
    enough memory for it. Assumes *header_name includes the ": ".
    Returns -1 if unable to find header. */
int find_header(char *http_header, char *header_name, char **header_value) {
    // Find the particular header from the HTTP header
    char *header_location;
    if ((header_location = strcasestr(http_header, header_name)) == NULL) {
        return -1; // invalid host header in HTTP request
    }
    header_location += strlen(header_name); // skip over to the host itself
    int header_location_i = 0;
    // Keep going until we've found the end of the line
    while (strncmp(header_location + header_location_i, "\r\n",
                   strlen("\r\n")) != 0) {
        header_location_i++;
    }

    // Allocate required memory for that
    /* Note: off-by-one concern: there is an additional ++ in the last
        iteration of the while for the '\0' */
    *header_value = malloc((header_location_i + 1) * sizeof(**header_value));
    assert(*header_value);
    // Copy over the header value instead of just pointing to it
    *header_value = strncpy(*header_value, header_location, header_location_i);
    (*header_value)[header_location_i] = '\0';

    return 0;
}
void free_find_header(char **header_value) {
    free(*header_value);
    *header_value = NULL;
}

/* Sets up a connection with the given target_host string
    and returns the file descriptor. Returns -1 on error. */
int connect_to_server(char *target_host) {
    int status, sockfd;
    // ---
    // STEP 1: Prepare required address information
    struct addrinfo hints, *results;
    // 1.1 Prepare hints to indicate
    memset(&hints, 0, sizeof hints); // as required by addrinfo struct
    hints.ai_family = AF_UNSPEC;     // get both IPv4 and IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP sockets
    // Before passing into getaddrinfo, must strip [] from the host header if
    // present
    int target_host_was_stripped = 0;
    if (target_host[0] == '[' && target_host[strlen(target_host) - 1] == ']') {
        // we're removing 2 characters, so size of malloc is -2 from what it
        // would be
        char *target_host_stripped =
            malloc((strlen(target_host) - 1) * sizeof(*target_host_stripped));
        // need to store the new length because strncpy doesn't add the '\0'
        // if it hits the n we specify, which it will always do here.
        int target_host_stripped_len = strlen(target_host) - 2;
        target_host_stripped = strncpy(target_host_stripped, target_host + 1,
                                       target_host_stripped_len);
        target_host_stripped[target_host_stripped_len] = '\0';
        target_host = target_host_stripped;
        target_host_was_stripped = 1;
    }
    // 1.2 Get local address information
    if ((status = getaddrinfo(target_host, REMOTE_HOST_PORT, &hints,
                              &results)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }
    if (target_host_was_stripped) {
        // we clean up the memory we allocated, and act as if nothing happened.
        // we don't need it anymore so setting it equal to NULL instead of the
        // original is fine.
        free(target_host);
        target_host = NULL;
    }

    // ---
    // STEP 2: Loop through results and connect to the first possible one
    // (note that addrinfo gives a linked list back)
    struct addrinfo *p;
    for (p = results; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1) {
            // whoops we can't use that one
            // so look for the next possible one
            perror("proxy: socket");
            continue;
        }

        // okay let's try connecting to it then
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            // oops never mind, time to clean up that connect
            // and find another one that could perhaps work
            close(sockfd);
            perror("server: connect");
            continue;
        }

        break; // successfully bound a socket!
    }

    freeaddrinfo(results); // don't need them anymore

    if (p == NULL) {
        // means we've failed to connect
        // as we didn't break early
        // and went through the whole list
        return -1;
    }

    return sockfd;
}

/* Sends over the string to the socket fd */
int send_request_to_host(int sockfd, char *buf, int num_bytes) {
    int amount_left_to_send = num_bytes, bytes_sent;
    char *ptr = buf;
    while (amount_left_to_send > 0) {
        bytes_sent = send(sockfd, ptr, amount_left_to_send, 0);
        if (bytes_sent == -1)
            return -1;
        amount_left_to_send -= bytes_sent;
        ptr += bytes_sent;
    }
    return 0;
}

/* Takes in response from the server socket fd, and sends that over
    piece by piece to the client socket fd after the response header
    is parsed. */
int process_and_send_response(int server_sockfd, int client_sockfd) {
    // PART 1: FINDING THE HEADER
    // Prepare an empty buffer
    char buf[BUFFER_SIZE];
    int buf_numbytes = 0;

    // Prepare header storage
    size_t header_max_size = INITIAL_HEADER_SIZE;
    size_t header_content_i = 0;
    char *header = malloc(header_max_size * sizeof(*header));
    assert(header);

    // Main loop; keep going until we find the empty line
    char *terminator = NULL;
    while ((terminator = search_n_substr(header, "\r\n\r\n", header_content_i)) == NULL) {
        // Read in as much as possible
        if ((buf_numbytes = recv(server_sockfd, buf, BUFFER_SIZE, 0)) ==
            -1 || buf_numbytes == 0) {
            perror("recv");
            return -1;
        }

        // Send the response as we go
        if (send_request_to_host(client_sockfd, buf, buf_numbytes) == -1) {
            return -1;
        }

        // Expand size of header if needed.
        if (header_content_i + buf_numbytes > header_max_size) {
            header_max_size *= 2;
            header = realloc(header, header_max_size * sizeof(*header));
            assert(header);
        }

        // Now that we have enough space, concat them.
        memcpy(header + header_content_i, buf, buf_numbytes);
        header_content_i += buf_numbytes;
    }
    // // for the sake of strncmp after this
    // if (header_max_size == header_content_i) {
    //     header_max_size *= 2;
    //     header = realloc(header, header_max_size * sizeof(*header));
    //     assert(header);
    // }
    // header[header_content_i] = '\0';

    // Clean up the header by removing what is beyond
    // Note the number of additional bytes we read beyond the header
    // so we know when to stop looking for more of the response.
    // Idea: Go backwards from end of header until found "\r\n\r\n".
    int additional_bytes_read = 0;
    char *curr = header + header_content_i - strlen("\r\n\r\n");
    while (strncmp(curr, "\r\n\r\n", strlen("\r\n\r\n")) != 0) {
        curr--;
        additional_bytes_read++;
    }
    *(curr + strlen("\r\n\r\n")) = '\0';

    // PART 2: FINDING THE CONTENT LENGTH
    char *content_length;
    if (find_header(header, "Content-Length: ", &content_length) == -1) {
        free(header);
        header = NULL;
        fprintf(stderr, "Invalid content-length header in HTTP request\n");
        return -1;
    }

    // PART 3: RECIEVE AND SEND THE REST OF THE BODY
    int content_length_int = atoi(content_length);
    int body_bytes_left = content_length_int - additional_bytes_read;
    free_find_header(&content_length);
    while (body_bytes_left > 0) {
        // Read in as much as possible
        if ((buf_numbytes = recv(server_sockfd, buf, BUFFER_SIZE, 0)) == -1 || buf_numbytes == 0) {
            perror("recv");
            free(header);
            header = NULL;
            return -1;
        }
        // Send the response as we go
        if (send_request_to_host(client_sockfd, buf, buf_numbytes) == -1) {
            free(header);
            header = NULL;
            return -1;
        }

        body_bytes_left -= buf_numbytes;
    }

    free(header);
    header = NULL;

    return content_length_int;
}

// the following is written by Jesslyn Andriono, probably adapted
// from above code by Bryan
/* Takes in response from the server socket fd, and sends it piece by piece
   to the client socket fd while also buffering the entire response in memory.
   The response is parsed to extract the header and determine content length.
   Useful for caching the full response after it is sent. */
int process_and_send_response_with_buffer(int server_sockfd, int client_sockfd, 
                                            char **response_buffer, int *content_length_out) {
    // PART 1: FINDING THE HEADER
    // Prepare an empty buffer
    char buf[BUFFER_SIZE];
    int buf_numbytes = 0;

    // Prepare header storage
    size_t header_max_size = INITIAL_HEADER_SIZE;
    size_t header_content_i = 0;
    char *header = malloc(header_max_size * sizeof(*header));
    assert(header);

    // Prepare response buffer for caching
    size_t response_buf_size = INITIAL_HEADER_SIZE;
    size_t response_buf_i = 0;
    *response_buffer = malloc(response_buf_size * sizeof(char));
    assert(*response_buffer);

    // Main loop; keep going until we find the empty line
    char *terminator = NULL;
    while ((terminator = search_n_substr(header, "\r\n\r\n", header_content_i)) == NULL) {
        // Read in as much as possible
        if ((buf_numbytes = recv(server_sockfd, buf, BUFFER_SIZE, 0)) == -1 || buf_numbytes == 0) {
            perror("recv");
            free(header);
            free(*response_buffer);
            *response_buffer = NULL;
            return -1;
        }

        // Send the response as we go
        if (send_request_to_host(client_sockfd, buf, buf_numbytes) == -1) {
            free(header);
            free(*response_buffer);
            *response_buffer = NULL;
            return -1;
        }

        // Expand size of header if needed.
        if (header_content_i + buf_numbytes > header_max_size) {
            header_max_size *= 2;
            header = realloc(header, header_max_size * sizeof(*header));
            assert(header);
        }

        // Now that we have enough space, concat them
        memcpy(header + header_content_i, buf, buf_numbytes);
        header_content_i += buf_numbytes;

        // Expand size of response buffer if needed
        if (response_buf_i + buf_numbytes > response_buf_size) {
            response_buf_size *= 2;
            *response_buffer = realloc(*response_buffer, response_buf_size);
            assert(*response_buffer);
        }

        // Store the data in the response buffer
        memcpy(*response_buffer + response_buf_i, buf, buf_numbytes);
        response_buf_i += buf_numbytes;
    }
    // Clean up the header by removing what is beyond
    // Note the number of additional bytes we read beyond the header
    // so we know when to stop looking for more of the response.
    // Idea: Go backwards from end of header until found "\r\n\r\n".
    int additional_bytes_read = 0;
    char *curr = header + header_content_i - strlen("\r\n\r\n");
    while (strncmp(curr, "\r\n\r\n", strlen("\r\n\r\n")) != 0) {
        curr--;
        additional_bytes_read++;
    }
    *(curr + strlen("\r\n\r\n")) = '\0';

    // PART 2: FINDING THE CONTENT LENGTH
    char *content_length;
    if (find_header(header, "Content-Length: ", &content_length) == -1) {
        free(header);
        free(*response_buffer);
        *response_buffer = NULL;
        fprintf(stderr, "Invalid content-length header in HTTP request\n");
        return -1;
    }

    // PART 3: RECIEVE AND SEND THE REST OF THE BODY
    int content_length_int = atoi(content_length);
    *content_length_out = content_length_int; 
    int body_bytes_left = content_length_int - additional_bytes_read;
    free_find_header(&content_length);

    while (body_bytes_left > 0) {
        // Read in as much as possible
        if ((buf_numbytes = recv(server_sockfd, buf, BUFFER_SIZE, 0)) == -1 || buf_numbytes == 0) {
            perror("recv");
            free(header);
            free(*response_buffer);
            *response_buffer = NULL;
            return -1;
        }

        // Send the response as we go
        if (send_request_to_host(client_sockfd, buf, buf_numbytes) == -1) {
            free(header);
            free(*response_buffer);
            *response_buffer = NULL;
            return -1;
        }

        // Expand size of response buffer if needed
        if (response_buf_i + buf_numbytes > response_buf_size) {
            response_buf_size *= 2;
            *response_buffer = realloc(*response_buffer, response_buf_size);
            assert(*response_buffer);
        }

        // Store the data in the response buffer
        memcpy(*response_buffer + response_buf_i, buf, buf_numbytes);
        response_buf_i += buf_numbytes;

        if (buf_numbytes > body_bytes_left)
            buf_numbytes = body_bytes_left;
        body_bytes_left -= buf_numbytes;

    }

    free(header);

    // Terminate the response buffer
    *response_buffer = realloc(*response_buffer, response_buf_i + 1);
    assert(*response_buffer);
    (*response_buffer)[response_buf_i] = '\0';

    return response_buf_i;
}

/* Gets the request-URI from the request and sets the value
    of *request_URI from that. */
void get_request_URI(char *request, char **request_URI) {
    request += strlen("GET ");
    int request_i = 0;
    while (*(request + request_i) != ' ') {
        request_i++;
    }
    *request_URI = malloc((request_i + 1) * sizeof(**request_URI));
    assert(*request_URI);
    *request_URI = strncpy(*request_URI, request, request_i);
    (*request_URI)[request_i] = '\0';
}
void free_get_request_URI(char **request_URI) {
    if (*request_URI) free(*request_URI);
    *request_URI = NULL;
}

/* Searches n characters of the string *haystack for
    the substring *needle, and returns a pointer to
    the start of the found *needle, and NULL otherwise.
    Only assumes that needle is a proper null-terminated string; doesn't assume that haystack is properly null-terminated.
    Naming convention inspired by man7.org strstr(3). */
char *search_n_substr(char *haystack, char *needle, int haystack_len) {
    if (haystack_len < strlen(needle)) {
        return NULL;
    }
    char *max = haystack + haystack_len - strlen(needle);
    char *curr = haystack;
    while (curr <= max && strncmp(curr, needle, strlen(needle)) != 0) {
        curr++;
    }
    assert(curr > max || strncmp(curr, needle, strlen(needle)) == 0);
    // Now distinguish whether we found it, or we didn't
    if (curr > max) {
        return NULL;
    } else {
        return curr;
    }
}