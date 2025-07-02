# Coding Portfolio 2025 - Project Descriptions
 
## JavaScript / HTML / CSS Projects
### Food Truck Database Website
Creating a website that can search through a database of food trucks near the Columbia University campus to help users find quick, delicious, and cheap meal options. Users can search by the name of the truck, the popular menu items, and by its location. Users are also able to view, add, or update existing data stored on the server. This project uses Python (Flask Web Framework), JavaScript, HTML, and CSS.

This project demonstrates **database operations**, **web development**, **server-client architecture**, **REST API design**, and **full-stack development**.

Video Walkthrough of Website:

[![Food Truck Database - Website Walkthrough](https://img.youtube.com/vi/RzrDEvxFioE/0.jpg)](https://www.youtube.com/watch?v=RzrDEvxFioE)

### Text Comparison Website
Creating a website that can compare two texts and show the word counts, character counts, readability in U.S. grade reading level (based on the Coleman-Liau Index), text similarity (based on the Cosine Similarity Percentage), and the counts of each word in both texts.

This project demonstrates **text processing algorithms**, **statistical analysis**, **string manipulation**, **web interface design**, and **mathematical formulas implementation**.

![Text Comparison Website - Home](https://github.com/cactus273/Portfolio/assets/86728731/479fc9cc-c118-47b5-8c31-b1018ead1e86)

![Text Comparison Website - Results](https://github.com/cactus273/Portfolio/assets/86728731/22586498-ffa5-40d2-b046-13d9a6f895b8)

![Text Comparison Website - Results Error](https://github.com/cactus273/Portfolio/assets/86728731/70b12261-0d33-4d2e-865d-b4f1ec7d6407)

### Fruit Hangman Game
Creating a hangman game played on the console where users are required to guess the secret fruit by guessing one letter at a time. Wrong words will add a strike to the hangman. The game ends when the fruit is guessed correctly or when the hangman is completed.

This project demonstrates **game logic implementation**, **input validation**, **string manipulation**, **conditional statements**, and **loop structures**.

![Fruit Hangman Game - Console View](https://github.com/cactus273/Portfolio/assets/86728731/05e2dd1d-3221-45a9-8c92-2407e8d1fd3c)

## C Projects
### HTTP Proxy Server (htproxy)
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

### Virtual Memory Management System
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

### 500 Digit Integer Calculator
Creating a calculator that can do addition, multiplication, and exponentiation with 500-digit integers. First, the number inputted is converted into a 'longint', a data type that stores each digit in an array and has a buddy variable indicating how many digits the number has. Next, functions that can add, multiply, and exponent 'longint' data types are made. The project was done as the first assignment for the class 'Foundations of Algorithms' and was used to test my understanding of arrays, strings, functions, structs, and the typedef facility.

This project demonstrates **data structures**, **algorithm design**, **large number arithmetic**, **memory management**, and **custom data types**.

### Text Generative Tool Algorithm 
Creating an algorithm that takes input sentences and prompts to return the most likely continuation of the prompt, if any, based on the input sentences. The input sentences are first processed letter by letter to create a prefix automaton plotting the frequency of each letter with the same exact prefix, like in the diagram below. Each prompt is then used to determine the suffix with the highest frequency. The project was done as the second assignment for the class 'Foundations of Algorithms' and was used to test my understanding of dynamic memory and linked data structures and extend my program design, testing, and debugging skills.

This project demonstrates **automaton design**, **frequency analysis**, **dynamic memory allocation**, **linked data structures**, and **pattern matching**.

![Text Generative Tool Algorithm - Conceptual Diagram](https://github.com/cactus273/Portfolio/assets/86728731/acecb398-6f27-49dd-8796-450a62f1cda2)

## Python Projects
### Falling Squares Matching Game
Creating a rhythm-based matching game with Pygame, similar to Dance Dance Revolution where colorful squares fall from the top of the screen and users must press corresponding arrow keys when squares reach target areas at the bottom. The game features a scoring system with Perfect/Miss feedback, sound effects, and increasing difficulty as the game progresses. Users aim to achieve the highest score and accuracy by timing their key presses precisely with the falling squares.

This project demonstrates **event-driven programming**, **real-time game loops**, **collision detection**, **graphics programming**, and **user interface design**.

![image](https://github.com/coms-w3132/final-project-cactus273/assets/86728731/19b00eda-b442-4abd-a769-e82a084be84f)
### Bejeweled Game with AI Player
Creating a matching game similar to Bejeweled, where within a grid of colored boxes, boxes with the same color are shifted to form a 2x2 group of matching colors and eliminated until none are left. In the diagram below, for example, shifting the green triangle down will form a 2x2 group of green triangles and will thus eliminate the group. On top of creating the mechanics of the game, determining the precedence of actions, and accommodating to edge cases, I also created an AI player that can process a grid and commit the least number of steps to finish the game.

This project demonstrates **game logic algorithms**, **AI decision making**, **grid manipulation**, **optimization strategies**, and **pathfinding algorithms**.

![Bejeweled Game with AI Player - Conceptual Diagram](https://github.com/cactus273/Portfolio/assets/86728731/a96a11d0-f7e6-43b7-a6d4-9184f519bb61)

### [Nerdle Game with AI Player](https://nerdlegame.com/) 
Creating a guessing game that is inspired by an existing number version of Wordle called Nerdle. The user inputs an equation to try to guess the secret equation. Seen in the example below, green indicates that the sign or number is correct and at the correct position. Yellow indicates that the number or sign exists within the equation but is not in the correct position. Whereas, grey indicates that the number or sign is not in the secret equation at all. I also created an AI Player that can play the game and guess correctly within an average of 6 guesses (for a 10 digit equation).

This project demonstrates **constraint satisfaction**, **AI search algorithms**, **pattern recognition**, **probability analysis**, and **heuristic optimization**.

![Nerdle - Example](https://github.com/cactus273/Portfolio/assets/86728731/382f9f74-4188-44a9-9e76-a249d344b7c9)

### Reach the Target Game
Creating a game where the user (X) is required to navigate through a grid of obstacles to reach the target (T) in the least number of steps. This project was made during the Practical Python Basics Workshop in March of 2023.

This project demonstrates **pathfinding algorithms**, **grid traversal**, **user input handling**, **game state management**, and **shortest path optimization**.

![Reach the Target Game - Terminal View](https://github.com/cactus273/Portfolio/assets/86728731/d30d993e-1c69-4fd9-9504-fa26820b2f1a)
