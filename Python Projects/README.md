# C Projects
## HTTP Proxy Server (htproxy)
Creating a caching HTTP/1.1 proxy server that sits between clients and origin servers, forwarding requests and responses while implementing intelligent caching mechanisms. The project includes basic proxy functionality, LRU cache implementation with 10 entries of 100 KiB each, HTTP Cache-Control header parsing to respect caching directives, and time-based cache expiration. The server handles TCP socket connections, parses HTTP requests and responses, and manages memory efficiently while maintaining proper logging of all operations.

This project demonstrates **socket programming**, **HTTP protocol implementation**, **caching algorithms**, and **memory management**.

**Team Members**
* **Bryan**: Stage 1 (Simple proxy) and partial Stage 4 (Expiration)
* **Me**: Stages 2 (Naive caching), 3 (Valid caching), and partial Stage 4 (Expiration)

**Usage**
```
# Compile the project
make

# Run without caching
./htproxy -p <port>

# Run with caching enabled
./htproxy -p <port> -c
```

## Virtual Memory Management System
Creating a virtual memory manager simulator that translates logical addresses to physical addresses using a Translation Lookaside Buffer (TLB) and page table with FIFO page replacement algorithm. The system manages a 4MB logical address space with 4KB pages mapped to a 1MB physical address space with 256 frames. Features include logical address parsing to extract page numbers and offsets, page table implementation with initial frame allocation, FIFO page replacement when physical memory is full, and TLB integration with LRU replacement policy for performance optimization. 

This project demonstrates **virtual memory concepts**, **address translation**, **page replacement algorithms**, **memory management**, **bit manipulation**, and **operating systems principles**.

**System Specifications**
- **Logical Address Space**: 4MB (2²² bytes), 4KB pages, 1024 pages total
- **Physical Address Space**: 1MB (2²⁰ bytes), 4KB frames, 256 frames total  
- **TLB**: 32 entries with LRU replacement policy
- **Page Replacement**: FIFO algorithm

**Usage**
```bash
make
./translate -f <input_file> -t <task>
```

## 500 Digit Integer Calculator
Creating a calculator that can do addition, multiplication, and exponentiation with 500-digit integers. First, the number inputted is converted into a 'longint', a data type that stores each digit in an array and has a buddy variable indicating how many digits the number has. Next, functions that can add, multiply, and exponent 'longint' data types are made. The project was done as the first assignment for the class 'Foundations of Algorithms' and was used to test my understanding of arrays, strings, functions, structs, and the typedef facility.

This project demonstrates **data structures**, **algorithm design**, **large number arithmetic**, **memory management**, and **custom data types**.

## Text Generative Tool Algorithm 
Creating an algorithm that takes input sentences and prompts to return the most likely continuation of the prompt, if any, based on the input sentences. The input sentences are first processed letter by letter to create a prefix automaton plotting the frequency of each letter with the same exact prefix, like in the diagram below. Each prompt is then used to determine the suffix with the highest frequency. The project was done as the second assignment for the class 'Foundations of Algorithms' and was used to test my understanding of dynamic memory and linked data structures and extend my program design, testing, and debugging skills.

This project demonstrates **automaton design**, **frequency analysis**, **dynamic memory allocation**, **linked data structures**, and **pattern matching**.

![Text Generative Tool Algorithm - Conceptual Diagram](https://github.com/cactus273/Portfolio/assets/86728731/acecb398-6f27-49dd-8796-450a62f1cda2)
