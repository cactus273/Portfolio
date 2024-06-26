# Coding Portfolio 2024 - Project Descriptions
 
## JavaScript / HTML / CSS Projects
### Food Truck Database Website
Creating a website that can search through a database of food trucks near the Columbia University campus to help users find quick, delicious, and cheap meal options. Users can search by the name of the truck, the popular menu items, and by its location. Users are also able to view, add, or update existing data stored on the server. This project uses Python (Flask Web Framework), JavaScript, HTML, and CSS.

Video Walkthrough of Website:

[![Food Truck Database - Website Walkthrough](https://img.youtube.com/vi/RzrDEvxFioE/0.jpg)](https://www.youtube.com/watch?v=RzrDEvxFioE)

### Text Comparison Website
Creating a website that can compare two texts and show the word counts, character counts, readability in U.S. grade reading level (based on the Coleman-Liau Index), text similarity (based on the Cosine Similarity Percentage), and the counts of each word in both texts.

![Text Comparison Website - Home](https://github.com/cactus273/Portfolio/assets/86728731/479fc9cc-c118-47b5-8c31-b1018ead1e86)

![Text Comparison Website - Results](https://github.com/cactus273/Portfolio/assets/86728731/22586498-ffa5-40d2-b046-13d9a6f895b8)

![Text Comparison Website - Results Error](https://github.com/cactus273/Portfolio/assets/86728731/70b12261-0d33-4d2e-865d-b4f1ec7d6407)


### Fruit Hangman Game
Creating a hangman game played on the console where users are required to guess the secret fruit by guessing one letter at a time. Wrong words will add a strike to the hangman. The game ends when the fruit is guessed correctly or when the hangman is completed.

![Fruit Hangman Game - Console View](https://github.com/cactus273/Portfolio/assets/86728731/05e2dd1d-3221-45a9-8c92-2407e8d1fd3c)



## C Projects
### 500 Digit Integer Calculator
Creating a calculator that can do addition, multiplication, and exponentiation with 500-digit integers. First, the number inputted is converted into a 'longint', a data type that stores each digit in an array and has a buddy variable indicating how many digits the number has. Next, functions that can add, multiply, and exponent 'longint' data types are made. The project was done as the first assignment for the class 'Foundations of Algorithms' and was used to test my understanding of arrays, strings, functions, structs, and the typedef facility.

### Text Generative Tool Algorithm 
Creating an algorithm that takes input sentences and prompts to return the most likely continuation of the prompt, if any, based on the input sentences. The input sentences are first processed letter by letter to create a prefix automaton plotting the frequency of each letter with the same exact prefix, like in the diagram below. Each prompt is then used to determine the suffix with the highest frequency. The project was done as the second assignment for the class 'Foundations of Algorithms' and was used to test my understanding of dynamic memory and linked data structures and extend my program design, testing, and debugging skills.

![Text Generative Tool Algorithm - Conceptual Diagram](https://github.com/cactus273/Portfolio/assets/86728731/acecb398-6f27-49dd-8796-450a62f1cda2)


## Python Projects
### Bejeweled Game with AI Player
Creating a matching game similar to Bejeweled, where within a grid of colored boxes, boxes with the same color are shifted to form a 2x2 group of matching colors and eliminated until none are left. In the diagram below, for example, shifting the green triangle down will form a 2x2 group of green triangles and will thus eliminate the group. On top of creating the mechanics of the game, determining the precedence of actions, and accommodating to edge cases, I also created an AI player that can process a grid and commit the least number of steps to finish the game.

![Bejeweled Game with AI Player - Conceptual Diagram](https://github.com/cactus273/Portfolio/assets/86728731/a96a11d0-f7e6-43b7-a6d4-9184f519bb61)


### [Nerdle Game with AI Player](https://nerdlegame.com/) 
Creating a guessing game that is inspired by an existing number version of Wordle called Nerdle. The user inputs an equation to try to guess the secret equation. Seen in the example below, green indicates that the sign or number is correct and at the correct position. Yellow indicates that the number or sign exists within the equation but is not in the correct position. Whereas, grey indicates that the number or sign is not in the secret equation at all. I also created an AI Player that can play the game and guess correctly within an average of 6 guesses (for a 10 digit equation).

![Nerdle - Example](https://github.com/cactus273/Portfolio/assets/86728731/382f9f74-4188-44a9-9e76-a249d344b7c9)


### Reach the Target Game
Creating a game where the user (X) is required to navigate through a grid of obstacles to reach the target (T) in the least number of steps. This project was made during the Practical Python Basics Workshop in March of 2023.

![Reach the Target Game - Terminal View](https://github.com/cactus273/Portfolio/assets/86728731/d30d993e-1c69-4fd9-9504-fa26820b2f1a)

