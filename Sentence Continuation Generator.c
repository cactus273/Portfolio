/*  Program to generate text based on the context provided by input prompts.

    Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
    September 2023, with the intention that it be modified by students
    to add functionality, as required by the assignment specification.
    All included code is (c) Copyright University of Melbourne, 2023.

    Made by, Jesslyn Andriono, October 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define NOSFMT "Number of statements: %d\n"                 // no. of statements
#define NOCFMT "Number of characters: %d\n"                 // no. of chars
#define NPSFMT "Number of states: %d\n"                     // no. of states
#define TFQFMT "Total frequency: %d\n"                      // total frequency
#define DOT "."                                             // to print ellipses

#define CRTRNC '\r'                     // carriage return character
#define START 1                         // compensate for the start null state
#define NEXT 1                          // to signify to skip once
#define NEXTNEXT 2                      // to signify to skip twice
#define START_SIZE 10                   // starting size when allocating memory
#define MAX_LEN 37                      // maximum line length of the prompt
#define ELLIPSE_LEN 3                   // number of dots to form the ellipses

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t;   // a state in an automaton
typedef struct node  node_t;    // a node in a linked list

struct node {                   // a node in a linked list of transitions has
    char*           str;        // ... a transition string
    state_t*        state;      // ... the state reached via the string, and
    node_t*         next;       // ... a link to the next node in the list.
};

typedef struct {                // a linked list consists of
    node_t*         head;       // ... a pointer to the first node and
    node_t*         foot;       // ... a pointer to the last node in the list.
} list_t;

struct state {                  // a state in an automaton is characterized by
    unsigned int    id;         // ... an identifier,
    unsigned int    freq;       // ... frequency of traversal,
    char            value;      // ... character value stored by state 
    int             visited;    // ... visited status flag, and
    list_t*         outputs;    // ... a linked list of output states.
};

typedef struct {                // an automaton consists of
    state_t*        ini;        // ... the initial state, and
    unsigned int    nid;        // ... the identifier of the next new state.
} automaton_t;

typedef struct {                    // an categorized stdin input consists of
    char*           sment;          // ... statement string (Stage 0)
    int             smentlen;       // ... length of statement string
    char*           prompt;         // ... prompt string (Stage 1)
    int             promptlen;      // ... length of prompt string
    int             comp;           // ... number of compressions
    char*           sec_prompt;     // ... prompt string (Stage 2)
    int             sec_promptlen;  // ... length of Stage 2 prompt string
} input_t;

typedef struct {                    // a catalogue of states consists of
    state_t**        states;        // ... pointer to an array of state pointers
    int*             size;          // ... pointer to current array capacity
} starray_t;

/* USEFUL FUNCTIONS ----------------------------------------------------------*/
int mygetchar(void);                // getchar() that skips carriage returns
input_t *create_input(void);        // initializes input type
void add_input(input_t *);          // reads the input
int count_sment(input_t *);         // counts number of statements read
int count_char(input_t *);          // counts number of characters read
state_t *create_state(void);        // initializes state type
list_t *create_list(void);          // creates the base for a linked list
starray_t *create_starray(void);    // initializes state array type
list_t *insert_at_foot(list_t *, state_t *);        
                                    // adds data to foot of list
void add_state(state_t *, state_t *, input_t *, int *, int *, starray_t *);
                                    // traverse the automaton to add frequency 
                                    //      or add a new state
state_t *find_state(state_t *, input_t *, int *);   
                                    // traverse the automaton to find the state 
                                    //      at the end of a prompt
state_t *find_highest_freq(node_t *, unsigned int *, state_t *);
                                    // traverse linked list to find the state 
                                    //      with the highest frequency
void continue_prompt(state_t *, int *);     
                                    // fill in the prompt's continuation
void print_prompt(int, input_t *, int *, int);      
                                    // prints the initial prompt
void do_prompt (state_t *, input_t *, int *);
                                    // prints the prompt and fills in the 
                                    //      rest of the sentence
state_t *find_lowest_ascii(node_t *, unsigned int *, state_t *);
                                    // traverse linked list to find the
                                    //     unvisited state with the lowest ASCII
state_t *find_leaf(state_t *);  // find the leaf state of an arc that has
                                    //     not been visited
void free_automaton(starray_t *, int );             
                                    // frees the automaton
void free_list(list_t *);           // frees a list
void free_state(state_t *);         // frees a state
void free_input(input_t *);         // frees the input

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    // Reads the input into the input type variable
    input_t *input = create_input();
    add_input(input);
    
    // Creates the starting state
    state_t *start = create_state();
    
    // To identify each state
    int id = 0;         
    
    // Creates the state array and adds the start state
    starray_t *starray = create_starray();
    *starray->size = START_SIZE;
    starray->states[id] = start;

    // Index of character in statement that has been added to automaton
    int tracker = 0;    

    // Adds characters in statement string into the automaton
    add_state(start, start, input, &id, &tracker, starray);

    // Prints statistics on the input and automaton
    printf(SDELIM, 0);
    printf(NOSFMT, count_sment(input));
    printf(NOCFMT, count_char(input));
    printf(NPSFMT, id + START);
    
    // Index of character in prompt that has been processed and printed
    tracker = 0;  
    
    // Prints the prompts and their continuation, if any
    printf(SDELIM, 1);  
    do_prompt(start, input, &tracker);
    
    // Compress and fill second prompt
    printf(SDELIM, 2);
    printf(NPSFMT, ((id + START) - input->comp));

    // Frees memory used
    free_automaton(starray, id);
    free_input(input);
    
    printf(THEEND);
    return EXIT_SUCCESS;
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

/*  An improved version of getchar(); skips carriage return characters.
    NB: Adapted version of the mygetchar() function by Alistair Moffat
*/
int mygetchar() {
    int c;
    while ((c=getchar()) == CRTRNC);

    return c;
}

/*  Takes nothing. Initializes input type, returns a pointer to the newly made 
    input variable.
*/
input_t *create_input() {
    input_t *input = (input_t*) malloc(sizeof(input_t));
    assert(input);
    
    // Initialize the statement, prompt, and second prompt strings
    int size = START_SIZE;
    input->sment = (char *) malloc(size * sizeof(char));
    assert(input->sment);
    input->smentlen = 0;
    
    input->prompt = (char *) malloc(size * sizeof(char));
    assert(input->prompt);
    input->promptlen = 0;
    
    input->comp = 0;
    input->sec_prompt = (char *) malloc(size * sizeof(char));
    assert(input->sec_prompt);
    input->sec_promptlen = 0;
    
    return input;
}

/*  Takes a pointer to an input variable and reads the stdin input into it, 
    returns nothing.
*/
void add_input(input_t *input) {
    // Size of memory allocated
    int size = START_SIZE;

    // To store the current and previous characters
    char current = '\0', prev = '\0';

    // To track the length of string, stage, and if the character is a separator
    int len = 0, stage = 0, separator = 0;

    // Flag of whether number of compressions is already stored
    int comp_stored = 0;
    char *comp_num = (char *) malloc(size * sizeof(char));
    assert(comp_num);

    // Reads until the end of file
    while ((current = mygetchar()) != EOF) {
        // Allocates more memory if needed
        if (len == size) {
            size *= 2;

            if (!stage) {
                input->sment = (char *) realloc(input->sment, 
                                                    size * sizeof(char));
                assert(input->sment);
            } else if (stage == 1) {
                input->prompt = (char *) realloc(input->prompt, 
                                                    size * sizeof(char));
                assert(input->prompt);
            } else if (stage == 2) {
                input->sec_prompt = (char *) realloc(input->sec_prompt, 
                                                        size * sizeof(char));
                assert(input->sec_prompt);
            }  
        }
        
        // Two newline characters mean input is now for the next stage
        if (prev == '\n' && current == '\n') {
            // At the end of each stage's input string, stores string length
            if (!stage) {
                input->smentlen = len;
            } else if (stage == 1) {
                input->promptlen = len;
            }
            
            // Increment the stage and reset the variables
            stage++;
            len = 0;     
            size = START_SIZE;

            // Since current is a separator, skips the newline character
            separator = 1;
        } else {
            separator = 0;
        }
        
        // Stores the character in the input variable according to its stage
        if (stage == 0) {
            input->sment[len++] = current;
        } else if ((stage == 1) && (!separator)) {
            input->prompt[len++] = current;
        } else if ((stage == 2) && (!separator) && (comp_stored)) {
            input->sec_prompt[len++] = current;
        }

        // Store number of compressions before storing the second prompts
        if ((stage == 2) && (!comp_stored) && (!separator)) {
            if (current != '\n') {
                // Store the whole number into a string
                if (len == size) {
                    size *= 2;
                    comp_num = (char *) realloc(comp_num, size * sizeof(char));
                }

                comp_num[len++] = current;
            } else {
                // Convert string into int, reset all variables
                comp_num[len] = '\0';
                input->comp = atoi(comp_num);
                free(comp_num);
                comp_num = NULL;
                len = 0;     
                size = START_SIZE;
                comp_stored = 1;
            }
        }
        
        prev = current;
    }
    
    // Stores string length of second prompt string
    input->sec_promptlen = len;
    
    // If input is not ended with a newline character, add a newline character
    if (input->sec_prompt[input->sec_promptlen - 1] != '\n') {
        input->sec_prompt[input->sec_promptlen] = '\n';
        input->sec_promptlen++;
    }
    
    return;
}

/*  Takes a pointer to an input variable, returns the number of statements read
*/
int count_sment(input_t *input) {
    int count = 0;

    // Counts the newline character at the end of each statement
    for (int i = 0; i < input->smentlen; i++) {
        if (input->sment[i] == '\n'){
            count++;
        }
    }

    return count;
}

/*  Takes a pointer to an input variable, returns the number of characters read
*/
int count_char(input_t *input) {
    int count = 0;

    // Counts characters in statement excluding newline characters
    for (int i = 0; i < input->smentlen; i++) {
        if (input->sment[i] != '\n'){
            count++;
        }
    }
    
    return count;
}

/*  Initializes a new state, returns the pointer to the newly made state
*/
state_t *create_state() {
    state_t *state = (state_t *) malloc(sizeof(state_t));
    assert(state);
    state->id = 0;
    state->freq = 0;
    state->visited = 0;
    state->value = '\0';

    // Linked list of outputs
    state->outputs = create_list();
    
    return state;
}

/*  Initializes a new linked list, returns a pointer to the newly made list
*/
list_t *create_list() {
    list_t *new_list = (list_t *) malloc(sizeof(list_t));
    assert(new_list);
    new_list->head = NULL;
    new_list->foot = NULL;
    
    return new_list;
}

/*  Initializes a new state array, returns a pointer to the newly made array
*/
starray_t *create_starray() {
    starray_t *starray = (starray_t*) malloc(sizeof(starray_t));
    starray->states = (state_t**) malloc(START_SIZE * sizeof(state_t*));
    starray->size = (int*) malloc(sizeof(int));
    assert(starray->size);
    
    return starray;
}

/*  Takes the pointers to the previous state, start state, input variable, id, 
    index tracker variable, and state array. Traverses through the automaton to 
    add frequency or add a new state, returns nothing.
*/ 
void add_state(state_t *prev, state_t *start, input_t *input, int *id, 
                int *tracker, starray_t *starray) {
    // Flag of whether an existing state matches the character from input
    int found = 0;

    // If the previous state has existing outputs
    if (prev->outputs->head) {
        node_t *current = prev->outputs->head;
        
        // Loops to check if any existing outputs match the character
        while (!found) {
            // If match is found
            if (input->sment[*tracker] == (current->state->value)) {
                // Increase the frequency
                prev->freq++;

                // If at the end of a statement
                if ((input->sment[*tracker + NEXT] == '\n') 
                    && ((*tracker + NEXTNEXT) < input->smentlen)) {
                    // Skip newline character and start from start state
                    (*tracker) += NEXTNEXT;
                    add_state(start, start, input, id, tracker, starray);
                
                // If at the end of the statement input string
                } else if ((input->sment[*tracker + NEXT] == '\n') 
                    && ((*tracker + NEXTNEXT) == input->smentlen)){
                    return;

                // Otherwise, move on to next character
                } else {
                    (*tracker)++;
                    add_state(current->state, start, input, id, tracker, 
                                starray);
                }
                
                found = 1;

            // If not a match
            } else {
                // If alternate outputs exist
                if (current->next) {
                    // Go to next output
                    current = current->next;
                } else {
                    // End search
                    break;
                }
            }
        }
    }
    
    // If no previous outputs match, create a new state
    if (!found) {
        // Creates a new state and input the data
        state_t *new_state = create_state();
        new_state->id = ++*id;
        new_state->value = input->sment[*tracker];

        // Increases the frequency of previous state
        prev->freq++;
        
        // Add the new state into the state array, allocate more memory if need
        if ((*id + START) == *starray->size) {
            *starray->size *= 2;
            starray->states = (state_t**) realloc(starray->states, 
                                                    *starray->size 
                                                    * sizeof(state_t*));
            assert(starray->states);
        }

        starray->states[*id] = new_state;
        prev->outputs = insert_at_foot(prev->outputs, new_state);
        
        // If at the end of a statement
        if ((input->sment[*tracker + NEXT] == '\n') 
            && ((*tracker + NEXTNEXT) < input->smentlen)) {
            // Skip newline character and start from start state
            (*tracker) += NEXTNEXT;
            add_state(start, start, input, id, tracker, starray);

        // If at the end of the statement input string
        } else if ((input->sment[*tracker + NEXT] == '\n') 
                    && ((*tracker + NEXTNEXT) == input->smentlen)){
            return;

        // Otherwise, move on to next character
        } else {
            (*tracker)++;
            add_state(new_state, start, input, id, tracker, starray);
        }
    }
    
    return;
}

/*  Takes pointers to the previous state, the input variable, and the index 
    tracker variable. Finds the state of the character at the end of the prompt.
    Returns the pointer to the state if present in the automaton. If not 
    present, returns a NULL pointer. 
*/
state_t *find_state(state_t *prev, input_t *input, int *tracker) {
    // Flag of whether the end of the prompt matches an arc in the automaton
    int found = 0;

    // Initialize the pointer to the end character's state as NULL
    state_t *end_state = NULL;

    // If the previous state has existing outputs
    if (prev->outputs->head) {
        node_t *current = prev->outputs->head;
        
        // Loops to check if any existing outputs match the character
        while (!found) {
            // If match is found
            if (input->prompt[*tracker] == current->state->value) {
                // If at the end of a prompt
                if (input->prompt[*tracker + NEXT] == '\n') {
                    (*tracker)++;

                    // But end of prompt is also the end of arc
                    if (!current->state->outputs->head) {
                        return NULL;
                    }
                    
                    // This state is the end state
                    return current->state;

                // Otherwise, move on to next character
                } else {
                    (*tracker)++;
                    end_state = find_state(current->state, input, tracker);
                }
                
                found = 1;
                
            // If not a match
            } else {
                // If alternate outputs exist
                if (current->next) {
                    // Go to next output
                    current = current->next;
                } else {
                    // End search
                    return NULL;
                }
            }
        }
    }
    
    // Returns the end state if found, NULL otherwise
    if (found) {
        return end_state;
    } else {
        return NULL;
    }
    
}

/*  Takes the head of a linked list, a pointer to the maximum frequency 
    variable, and a pointer to the maximum state. Finds the state with the
    highest frequency and if there is a tie, the state with the higher ASCII 
    value. Returns a pointer to said state.
*/
state_t *find_highest_freq(node_t *head, unsigned int *max_freq, 
                           state_t *max_state) {
    // If node is NULL, return the existing max_state
    if (!head) {
        return max_state;
    }

    // If the freq is higher than the max or if equal but has greater ASCII
    if ((head->state->freq > *max_freq) 
        || ((head->state->freq == *max_freq) 
            && (head->state->value > max_state->value)))  {
        *max_freq = head->state->freq;
        max_state = head->state;
    } 

    // Recurse to traverse through the linked list
    max_state = find_highest_freq(head->next, max_freq, max_state);

    return max_state;
}

/*  Takes a pointer to the previous state and a pointer to the length of the
    current prompt. Fills in the rest of the prompt, or stopping when the 
    maximum line length of a prompt is reached. Returns nothing.
*/
void continue_prompt(state_t *prev, int *len) {
    // To track the maximum frequency and the state it is contained in
    unsigned int max_freq = 0;
    state_t *max_state = create_state();
    state_t *temp_state = max_state;
    
    //  If the previous state has outputs
    if (prev->outputs->head) {
        // If the previous state has multiple outputs
        if (prev->outputs->head->next) {
            // Find the state with the highest frequency
            max_state = find_highest_freq(prev->outputs->head, &max_freq, 
                                            max_state);
        
        // If the previous state has only one output
        } else {
            // The one output is automatically the maximum state
            max_state = prev->outputs->head->state;
        }

        // Prints if the prompt's length is still less than the max line length
        if (*len < MAX_LEN) {
            printf("%c", max_state->value);
            (*len)++;

        // Returns if the max line length is reached
        } else {
            return;
        }
    }
    
    // If the maximum state has outputs, continue the prompt
    if (max_state->outputs->head) {
        if (*len < MAX_LEN) {
            continue_prompt(max_state, len);
        } 
    }

    free(temp_state);
    return;
}

/*  Takes the starting index of the current prompt, a pointer to the input 
    variable, a pointer to the length of the current prompt, and the end index 
    of the current prompt. Prints the prompt until its end index, or until the 
    maximum line length is reached. Returns nothing.
*/
void print_prompt(int start_index, input_t *input, int *len, int end_index) {
    // Determine the start of the current prompt
    int index = start_index;

    // Loops while index is not at the end and the prompt's length is not at max
    while ((index <= end_index ) && (*len < MAX_LEN)) {
        // Prints the character from the prompt
        if (input->prompt[index] != '\n') {
            printf("%c", input->prompt[index]);
        }
        
        // Increments the current prompt line length and index
        (*len)++;
        index++;
    }
    
    // Prints the ellipses after the prompt if there is line space
    for (int i = 0; i < ELLIPSE_LEN; i++) {
        if (*len < MAX_LEN) {
            printf(DOT);
            (*len)++;
        }
    }
    
    return;
}

/*  Takes pointers to the start state, the input variable, and the index tracker
    variable. Processes, prints, and fills in all the prompts, if possible.
    Returns nothing.
*/
void do_prompt (state_t *start, input_t *input, int *tracker) {
    // Loops while there is still another prompt
    while (*tracker < input->promptlen) {
        // The start index within the whole prompt string for the current prompt
        int start_index = *tracker;
        
        // To track the line length of the current prompt
        int len = 0;

        // Determine the state of the last character in the prompt, if any
        state_t *end_state = find_state(start, input, tracker);

        // Determine the end of the prompt, or the last character with no match
        int end_index = *tracker;
        
        // If the end state is not found
        if (!end_state) {
            // Print until the prompt's end or the last character without match
            print_prompt(start_index, input, &len, end_index);
            printf("\n");
            
        } else {
            // Print until the end of the prompt
            print_prompt(start_index, input, &len, end_index);
            
            // If there is still line space, continue the prompt
            if (len < MAX_LEN) {
                continue_prompt(end_state, &len);
            }
            
            printf("\n");
        }

        // Increment index tracker to the end of the prompt, even if no match
        while (input->prompt[*tracker - 1] != '\n') {
            (*tracker)++;
        }
    }
    
    return;
}

/*  Takes the head of a linked list, a pointer to the lowest ASCII 
    variable, and a pointer to the lowest state. Finds the state that has not 
    been visited with the lowest ASCII. Returns a pointer to said state.
*/
state_t *find_lowest_ascii(node_t *head, unsigned int *min_ascii, 
                           state_t *min_state) {
    // If node is NULL, return the existing min_state
    if (!head) {
        return min_state;
    }

    // If the ASCII is lower than the min and not yet visited
    if (((unsigned int) head->state->value < *min_ascii) 
            && (!head->state->visited))  {
        *min_ascii = (unsigned int) head->state->value;
        min_state = head->state;
    } 

    // Recurse to traverse through the linked list
    min_state = find_lowest_ascii(head->next, min_ascii, min_state);

    return min_state;
}

/*  Takes a pointer to the previous state. Find the leaf state at the end of an
    arc that has the lowest ASCII and has not been visited. Returns the pointer
    to said leaf state.
*/
state_t *find_leaf(state_t *prev) {
    // To track the minimum ASCII and the state it is contained in
    unsigned int min_ascii = 0;
    state_t *min_state = create_state();
    state_t *temp_state = min_state;
    state_t *leaf_state = NULL;

    //  If the previous state has outputs
    if (prev->outputs->head) {
        // If the previous state has multiple outputs
        if (prev->outputs->head->next) {
            // Find the state with the highest frequency
            min_state = find_lowest_ascii(prev->outputs->head, &min_ascii, 
                                            min_state);
        
        // If the previous state has only one output
        } else {
            min_state = prev->outputs->head->state;
        }

    // If the previous state has no outputs    
    } else {
        return NULL;
    }

    // If the state is a leaf state (end of an arc)
    if (!min_state->outputs->head) {
        leaf_state = min_state;
    } else {
        leaf_state = find_leaf(min_state);
    }

    free(temp_state);
    return leaf_state;
}

/*  Takes a pointer to the state array and id. Frees all states in an automaton.
*/
void free_automaton(starray_t *starray, int id) {
    if (!starray) {
        return;
    }
    
    // Free each state
    for (int i = id; i >= 0; i--) {
        free_state(starray->states[i]);
        starray->states[i] = NULL;
    }
    
    // Free the state array
    free(starray->states);
    starray->states = NULL;
    free(starray);
    starray = NULL;
    
    return;
}

/*  Takes a pointer to a state. Frees the state. Returns nothing.
*/
void free_state(state_t *state) {
    if (!state) {
        return;
    }
    
    // Free the linked list in the state, then the state itself
    free_list(state->outputs);
    state->outputs = NULL;
    free(state);
    state = NULL;

    return;
}

/*  Modified from PPSAA, Chapter 10, Courtesy of Alistair Moffat
    https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
    Takes a pointer to a list. Frees the list. Returns nothing. 
*/ 
void free_list(list_t *list) {
    if (!list) {
        return;
    }
    
    // Free the elements of the list
    node_t *curr = list->head, *prev;

    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
        prev = NULL;
    }
    
    // Free the list itself
    free(list);
    list = NULL;

    return;
}

/*  Takes a pointer to a input variable. Frees the input. Returns nothing.
*/
void free_input(input_t *input) {
    // Free each category of input, then the input itself
    free(input->sment);
    input->sment = NULL;
    free(input->prompt);
    input->prompt = NULL;
    free(input->sec_prompt);
    input->sec_prompt = NULL;
    free(input);
    input = NULL;

    return;
}

/*  Modified from PPSAA, Chapter 10, Courtesy of Alistair Moffat
    https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
    Takes pointers to a list and to a state. Adds the state at the end of a 
    list. Returns the pointer to the list. 
*/
list_t *insert_at_foot(list_t *list, state_t *value) {
    node_t *new = (node_t *) malloc(sizeof(*new));
    assert(list != NULL && new != NULL);
    new->state = value;
    new->next = NULL;

    // Add value to the end of list
    if (list->foot == NULL) {
        // If this is the first insertion into the list
        list->head = new;
        list->foot = new;
    } else {
        list->foot->next = new;
        list->foot = new;
    }

    return list;
}


/* THE END -------------------------------------------------------------------*/