# HTTP Proxy Server (htproxy)

A simple caching HTTP/1.1 proxy server implemented in C.

## Team Members
- **Bryan**: Stage 1 (Simple proxy) and partial Stage 4 (Expiration)
- **Me**: Stages 2 (Naive caching), 3 (Valid caching), and partial Stage 4 (Expiration)

## Features

### Stage 1: Simple Proxy
- Basic HTTP request proxying without caching
- TCP socket listening on specified port
- Request forwarding to origin servers on port 80
- Response relaying back to clients
- Proper logging of connections and requests

### Stage 2: Naive Caching
- LRU (Least Recently Used) cache with 10 entries of 100 KiB each
- Caches all requests under 2000 bytes with responses ≤ 100 KiB
- Cache hit/miss detection and serving
- Automatic cache eviction when full

### Stage 3: Valid Caching
- Respects HTTP Cache-Control headers
- Prevents caching when headers contain:
  - `private`
  - `no-store`
  - `no-cache`
  - `max-age=0`
  - `must-revalidate`
  - `proxy-revalidate`
- Proper parsing of complex Cache-Control directives

### Stage 4: Expiration
- Cache entry expiration based on `max-age` directive
- Automatic refetching of stale entries
- Time-based cache invalidation
- Proper handling of expired content

## Usage

```bash
# Compile the project
make

# Run without caching
./htproxy -p <port>

# Run with caching enabled
./htproxy -p <port> -c
```

## Project Structure
- `main.c` / `main.h` - Main program entry and core logic
- `stage1.c` / `stage1.h` - Basic proxy functionality
- `stage2.c` / `stage2.h` - Naive caching implementation
- `stage3.c` / `stage3.h` - Cache-Control header parsing
- `stage4.c` / `stage4.h` - Cache expiration logic
- `Makefile` - Build configuration

## Notes
This project demonstrates understanding of:
- Socket programming in C
- HTTP protocol implementation
- Caching strategies and algorithms
- Memory management and safety
- Network programming concepts
