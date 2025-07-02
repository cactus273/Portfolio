# COMS W3132 Individual Project

## Author
Jesslyn Andriono

## Project Title
Falling Squares Matching Game

## Project Description
I want to make a game that is similar to the arcade game _Dance Dance Revolution_ where arrows appear and when the arrows match the outline, the user needs to press the up, down, left, or right key accordingly. I want to work on it because I like video games and rhythm games in particular because it is satisfying. I am also interested in game development so it is the topic that I will enjoy the most. I think that most people have played some form of a similar game and thus it will be interesting to see how it can be made on desktop. It should be fun so hopefully other people can enjoy it.

![image](https://github.com/coms-w3132/final-project-cactus273/assets/86728731/19b00eda-b442-4abd-a769-e82a084be84f)

(In the image above the arrows move from the bottom to the top. In my implementation, it will be from the top to the bottom and the outlines will be at the bottom)


## Timeline

*To track progress on the project, we will use the following intermediate milestones for your overall project. Each milestone will be marked with a tag in the git repository, and we will check progress and provide feedback at key milestones.*

| Date               | Milestone                                                                                              | Deliverables                | Git tag    |
|--------------------|--------------------------------------------------------------------------------------------------------|-----------------------------|------------|
| **March&nbsp;29**  | Submit project description                                                                             | README.md                   | proposal   |
| **April&nbsp;5**   | Update project scope/direction based on instructor/TA feedback                                         | README.md                   | approved   |
| **April&nbsp;12**  | Basic project structure with empty functions/classes (incomplete implementation), architecture diagram | Source code, comments, docs | milestone1 |
| **April&nbsp;19**  | Progress on implementation (define your own goals)                                                     | Source code, unit tests     | milestone2 |
| **April&nbsp;26**  | Completely (or partially) finished implementation                                                      | Source code, documentation  | milestone3 |
| **May&nbsp;10**    | Final touches (conclusion, documentation, testing, etc.)                                               | Conclusion (README.md)      | conclusion |

*The column Deliverables lists deliverable suggestions, but you can choose your own, depending on the type of your project.*

## Requirements, Features and User Stories
My aim is to create the game with several important functionalities:
- To have the arrows be working, appearing from the top of the screen and falling.
- To have the outlines be receptive of the arrows so when the arrow fits the outline the score is recorded, a miss is recorded otherwise.
- The arrows fall in a rhythm, possibly accompanied by a song to which the rhythm is based on.
- Having some arrows be single tap and some hold arrows that are longer and the key needs to be held for a few seconds.
- Depending on how closely to the outline the arrow is when the key is pressed, there are different categories with different scores such as Perfect, Good, and Ok.
- The score is kept and there are statistics shown after of the accuracy, etc.

Overall, the users will strive to press the keys when the arrow perfectly matches the outline and also hold the key for the perfect amount of time to get the highest score possible, also the highest accuracy and the lowest number of misses.

## Technical Specification
I believe this idea is doable with Pygame.

To implement the feature where the arrow perfectly matches the outline, I will define the target area by specifying a rectangular region on the screen where the arrow needs to perfectly match and determine the position of the arrow when the user presses, such as using the rect attribute. I will then compare its position with the position of the target area, determining whether it fits in the "perfect" category by being a certain range of the target area's position, or just the "good", which will be more lenient. 

To design the sequence of arrows, I am interested to make a linked list where each node has two values, first the arrow that is outputted and second, the response of the user. If there is no response within a certain time of the arrow approaching the target area, then it will be considered a miss. After, it will help with scoring as the two values will be compared, whether it matches or not.

I want to use existing music to base the sequence of arrows, or possibly referring to a _Dance Dance Revolution_ sequence or a similar mobile game such as _Tap Tap_.

## System or Software Architecture Diagram
![Software Architecture Diagram](https://github.com/coms-w3132/final-project-cactus273/assets/86728731/31fa62a3-93a3-41dc-b9bb-0a39d28341ac)


## Development Methodology
- Envisioning the most ideal version of the game, imagining what features would be included and how the data structures are able to help implement it. Use this as a reference for creating the software.
- Learn PyGame as I was not familiar with it before.
- Focus on bringing the visual elements on the page first, such as squares falling, lanes, and target areas
- Upon learning PyGame, identifying the challenges that may be too difficult to realistically implement such as actually using arrows, hold functionalities, and arranging the arrows to fall in accordance to a song. (I realized that arrows are specific to _Dance Dance Revolution_ and other similar games such as _Tap Tap_ use circles, similar to my use of squares)
- Move on to making it functional, such as identifying when squares are within target areas
- Keep track and store the data of scores, misses, high scores, etc.
- Incorporate the data structures learned in class for more efficiency
- Develop more features to make the game more robust
- Do finishing touches such as adding sound effects

- Throughout: **Commit updates to GitHub** to store previous versions in case of future bugs, to have a plan to fall back to. **Test manually** each new feature as it is being developed, to see whether it merges well with previous features or not.


## Challenges and Roadblocks
I experienced various challenges in actually getting the game to where I envisioned it in the first place. I was not able to create a specific series of arrows based on a song as it is too complicated for my abilities. Hence, I ended up making the arrows, now replaced with colorful squares as it felt better suited for the desktop setting, fall in random intervals and random order. This was I believe 
that the user may find more enjoyment as there is no one set sequence. In addition, I was thinking of implementing a Perfect/Excellent/Good/Miss system. However, I realized that rather than aiming for a perfect match, considering that there is no music used in this version, it would be better to speed up the rate at which squares fall and using just a Perfect/Miss binary. 

## Additional Resources
PNG Sites such as parspng.com and Sound Effect Sites such as pixabay.com

## Conclusion and Future Work
Overall, I really stepped out of my comfort zone through this project, as I have been interested in game development but have not actually tried it. Through this experience, I was able to become more realistic in my development because I realized that much of the features I envisioned were quite advanced to implement. I believe that in one way or another, despite having to make various changes to the concept I initially proposed, I still have met my goals and I have undergone a significant amount of learning in game development. In the future, with more practice and experience, I would like to develop this project to fit better with my initial concept and even think of features that do not currently exist in _Dance Dance Revolution_, to make the gameplay both more challenging, unique, and fun.
