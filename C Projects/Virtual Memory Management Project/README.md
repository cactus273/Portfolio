# Virtual Memory Management Project

## Overview
This project implements a virtual memory manager simulator that translates logical addresses to physical addresses using a Translation Lookaside Buffer (TLB) and page table with FIFO page replacement.

Completed: 20 April 2025

## System Specifications
- **Logical Address Space**: 4MB (2²² bytes), 4KB pages, 1024 pages total
- **Physical Address Space**: 1MB (2²⁰ bytes), 4KB frames, 256 frames total  
- **TLB**: 32 entries with LRU replacement policy
- **Page Replacement**: FIFO algorithm

## Tasks Implemented

### Task 1: Logical Address Parser
- Extracts page number (10 bits) and offset (12 bits) from 32-bit logical addresses
- Masks the leftmost 10 bits, processes rightmost 22 bits

### Task 2: Page Table Implementation
- Implements page table with 1024 entries
- Handles initial frame allocation in increasing frame order
- Manages page faults when free frames are available

### Task 3: Page Replacement Algorithm
- Implements FIFO page replacement when all frames are allocated
- Evicts least recently loaded pages to make room for new pages

### Task 4: TLB Integration
- Adds TLB with LRU replacement policy
- Handles TLB hits/misses and flushes entries for evicted pages
- Optimizes address translation performance

## Usage
```bash
make
./translate -f <input_file> -t <task>
```

Where `<task>` is one of: `task1`, `task2`, `task3`, `task4`

## Input Format
Input file contains one logical address per line (32-bit unsigned integers).

Example:
```
30023
10001
20007
```

## Project Structure
```
├── Makefile
├── README.md
├── main (main.c)
├── main (main.h)
├── task1.c / task1.h
├── task2.c / task2.h
├── task3.c / task3.h
├── task4.c / task4.h
├── translate (executable)
├── temp-output
└── cases/ (test cases)
```

## Notes
- All memory accesses are read-only operations
- Single process simulation (no process scheduling)
- Deterministic output required for automated testing
- Exit code 0 expected for successful execution
