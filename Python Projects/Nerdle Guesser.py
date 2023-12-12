# Nerdle (Number Wordle) and AI Guesser
# Made by Jesslyn Andriono, May 2023

import random
from collections import Counter

DEF_DIFFIC = 10
MAX_TRIALS = 50
SEC_GUESS_MAX_TRIALS = 20
MIN_VALUE = 1
MAX_VALUE = 99
NUM_OF_VALUES = 3
NUM_OF_OPERATORS = 2
# Index within tuple
POSI = 0
CHAR = 1
STATUS = 2
OPERATORS = "+-*%"
EQUALITY = "="
DIGITS = "0123456789"
GREEN = "green"
YELLO = "yellow"
GREYY = "grey"

def create_better_guess(all_info, difficulty=DEF_DIFFIC):
    '''
    Takes information built up from past guesses that is stored in `all_info`, 
    and uses it as guidance to generate a new guess of length `difficulty`. 
    '''
    # Creates a first guess that tests operators and several possible numbers
    if all_info == []:
        guess = first_guess(difficulty)
        return guess
    
    # Determines the operators used in the equation
    possible_operators = find_operators(all_info)
    
    # Formats a second guess, value-operator-value-operator-value-equal-answer,
    # using information gained from first guess
    if len(all_info) == 1:
        for i in range(SEC_GUESS_MAX_TRIALS):
            guess = sec_guess(possible_operators, difficulty)
            if guess is not None:
                return guess
    
    # Determines list of positions and characters with green and yellow
    # statuses respectively
    green = sort_info(all_info, GREEN)
    yellow = sort_info(all_info, YELLO)
    
    # Determines the characters that are absent from the secret equation
    grey = absent_chars(all_info)
    
    # Determined if the position of the equal sign is found 
    equality_found = find_equality(all_info)
    
    # Determines positions without confirmed values (green status)
    empty_positions = list(range(difficulty))
    
    # Creates a list of length difficulty with empty values
    equation = []
    for i in range(difficulty):
        equation.append("")
    
    # Fills in the characters with confirmed positions (green status)
    for tup in green:
        equation[tup[POSI]] = tup[CHAR]
        empty_positions.remove(tup[POSI])
    
    # Remove absent characters from possible values
    possible_digits = delete_grey(DIGITS, grey)
    possible_operators = delete_grey(possible_operators, grey)
    
    # Tries to find the ideal guess that passes restrictions within MAX_TRIALS
    for i in range(MAX_TRIALS):
        # Create a deep copy of equation with confirmed positions
        equation_copy = []
        for i in equation:
            equation_copy.append(i)
        
        # Fill in the rest of the equation
        for pos in empty_positions:
            # Determines the possible values
            possible_value = possible_digits + possible_operators
            if not equality_found:
                possible_value += EQUALITY

            possible_value = list(possible_value)
            
            # Removes characters that are confirmed to not be in the position
            # from possible values (yellow status)
            
            yellow_char = yellow_posi(yellow, pos)
            for char in yellow_char:
                if char in possible_value:
                    possible_value.remove(char)
                    
            # Allocate a random character from possible values to position
            equation_copy[pos] = random.choice(possible_value)

        # Combine equation list into a string and returns the guess if ideal
        equation_copy = "".join(equation_copy)  
        if passes_restrictions(equation_copy, all_info):
            return equation_copy

    # If no ideal guess is found, return last version of guess
    return equation_copy


def passes_restrictions(guess, all_info):
    '''
    Tests a `guess` equation against `all_info`, a list of known restrictions, 
    one entry in that list from each previous call to set_colors(). Returns 
    True if that `guess` complies with the collective evidence imposed by 
    `all_info`; returns False if any violation is detected. Does not check the 
    mathematical accuracy of the proposed candidate equation.
    '''
    # Determines list of positions and characters with green and yellow
    # statuses respectively
    green = sort_info(all_info, GREEN)
    yellow = sort_info(all_info, YELLO)
    
    # Determines the characters that are absent from the secret equation
    grey = absent_chars(all_info)
    
    # Checks if all confirmed green characters are at the right position
    for tup in green:
        if guess[tup[POSI]] is not tup[CHAR]:
            return False
    
    # Checks if any yellow character is present at the same position it was 
    # before and if any yellow character is not present in the guess
    for tup in yellow:
        if guess[tup[POSI]] is tup[CHAR]:
            return False
        elif tup[CHAR] not in guess:
            return False
    
    # Checks if any absent characters are used in the guess
    for char in guess:
        if char in grey:
            return False
    
    return True


def sort_info(all_info, color):
    '''
    Takes 'all_info' restrictions and a color. Creates a list of tuples 
    (position, character, status) with that color status, without duplicates. 
    Returns the sorted list.
    '''
    color_info = []
    for trial in all_info:
        for tup in trial:
            if (tup[STATUS] is color) and (tup not in color_info):
                color_info.append(tup)
    
    # Returns sorted list of tuples with that color status
    return sorted(color_info)


def absent_chars(all_info):
    '''
    Takes 'all_info' restrictions. Returns a sorted list of characters that are 
    confirmed to be absent from the secret equation. 
    '''
    absent = []
    for trial in all_info:
        guess = ""
        grey = ""
        for tup in trial:
            guess += tup[CHAR]
            if tup[STATUS] is GREYY:
                grey += tup[CHAR]
        
        # If char is grey and only used once in guess, it is absent from secret
        for char in grey:
            if (guess.count(char) == 1) and (char not in absent):
                absent.append(char)
    
    # Returns sorted list of absent characters
    return sorted(absent)  


def yellow_posi(yellow, posi):
    '''
    Takes a list of tuples with yellow status from all_info and a position. 
    Returns a list of characters with yellow status in the specific position.
    '''
    char = []
    for tup in yellow:
        if (tup[POSI] is posi) and (tup[CHAR] not in char):
            char.append(tup[CHAR])
    
    # Returns sorted list with no duplicate values
    return sorted(char)


def delete_grey(value, grey):
    '''
    Takes a value category (digits or operators) and a list of absent 
    characters. Returns a string of possible values in the category after
    removing absent characters.
    '''
    possible_values = ""
    for i in value:
        if i not in grey:
            possible_values += i
    
    return possible_values


def first_guess(difficulty):
    '''
    Takes the FoCdle's difficulty. Creates a first guess that determines what 
    operator signs are used while also attempting to eliminate as much numbers 
    as possible from possible values. Returns the first guess equation.
    '''
    guess = "12" + OPERATORS + "0" + EQUALITY
    for i in range(3, difficulty - 5):
        guess += str(i)
    
    return guess


def sec_guess(poss_operators, difficulty=DEF_DIFFIC):
    '''
    Takes the difficulty of the FoCdle and possible operators, attempts to 
    generate a valid equation of the difficulty length while considering only 
    possible operators. Returns the equation if a valid equation is found 
    within the maximum number of trials, returns None otherwise.
    '''
    # Attempts to find an equation until maximum number of trials is reached
    for i in range(MAX_TRIALS):
        # Generates an equation
        equation = generate_equation(poss_operators)
        answer = eval(equation)
        equation = equation + EQUALITY + str(answer)
        
        # Checks if length of equation and answer value is valid
        # Stops search for equation by returning valid equation if found
        if (len(equation) is difficulty) and (answer > 0):
            return equation
    
    # Returns None if equation is not found        
    return None


def generate_equation(poss_operators):
    ''' 
    Takes possible operators, generates an equation in string form with the 
    format value-operator-value-operator-value. Returns the equation string.
    '''
    # Adds values and operators to the string in order
    equation = ""
    operators = []
    
    # Create deep copy of possible operators
    for op in poss_operators:
        operators.append(op)
        # If only one operator is in possible operators (meaning there is two
        # of the same operator used in equation) double of the same operator
        # is added to the list
        if len(poss_operators) == 1:
            operators.append(op)
        
    for i in range(NUM_OF_VALUES):
        equation += str(random.randint(MIN_VALUE, MAX_VALUE))
        # If two different operators are used, ensures that both are used
        if i < NUM_OF_OPERATORS:
            chosen = random.choice(operators)
            equation += chosen
            operators.remove(chosen)
    
    return equation


def find_equality(all_info):
    '''
    Takes information from past trials in all_info to determine if the position
    of the equal sign has been found. Returns True if found, False otherwise.
    '''
    for trial in all_info:
        for tup in trial:
            if (tup[CHAR] is EQUALITY) and (tup[STATUS] is GREEN):
                return True
    
    return False


def find_operators(all_info):
    '''
    Takes restrictions in all_info. Determines which operators are in the
    equation. Returns a string of operators present in the equation.
    '''
    operator = []
    for op in OPERATORS:
        operator.append(op)
    
    for trial in all_info:
        guess = ""
        grey = ""
        for tup in trial:
            guess += tup[CHAR]
            if tup[STATUS] is GREYY:
                grey += tup[CHAR]

        # If char is grey and only used once in guess, it is absent from secret
        for char in grey:
            if (guess.count(char) == 1) and (char in operator):
                operator.remove(char)
                
    return "".join(operator)


def set_colors(secret, guess):
    '''
    Compares the latest `guess` equation against the unknown `secret` one. 
    Returns a list of three-item tuples, one tuple for each character position 
    in the two equations:
        -- a position number within the `guess`, counting from zero;
        -- the character at that position of `guess`;
        -- one of "green", "yellow", or "grey", to indicate the status of
           the `guess` at that position, relative to `secret`.
    The return list is sorted by position.
    '''
    # Creates a dictionary with counts of each character in the secret equation
    count = Counter(secret)
    # Creates a list of character positions without a tuple 
    positions_left = list(range(len(secret)))
    
    # Checks if any guess positions are green (correct position and character)
    # Position, character, and status is added to the list if green
    status = []
    for i, char in enumerate(secret):
        if guess[i] == char:
            status.append((i, char, GREEN))
            count[char] -= 1
            positions_left.remove(i)
    
    # For character positions that are not green, checked if yellow or grey
    # Position, character, and status is added to the list
    for p in positions_left:
        # If character is present in guess, only leftmost are yellow
        if (guess[p] in secret) and (count[guess[p]] > 0):
            status.append((p, guess[p], YELLO))
            count[guess[p]] -= 1
        else:
            status.append((p, guess[p], GREYY))
    
    # Return the list of tuples sorted by position
    return sorted(status)
