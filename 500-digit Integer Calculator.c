/*  Program to perform multi-digit integer arithmetic.

    Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
    August 2023, with the intention that it be modified by students
    to add functionality, as required by the assignment specification.
    All included code is (c) Copyright University of Melbourne, 2023

    Made by Jesslyn Andriono, September 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */
#define ASCII_ZERO 48   /* ASCII value of zero */
#define NULL_ALLOC 1    /* allocation of null character at the end of string */

#define INT_TEN  10     /* integer 10 */
#define LEN_ONE  1     /* integer 1 for longint*/


typedef struct{
    int length;
    int digit[INTSIZE];
} longint_t;


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
    longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t *var);
void do_assign(longint_t *var1, longint_t *var2);
void do_plus(longint_t *var1, longint_t *var2);
void zero_vars(longint_t vars[]);
longint_t parse_num(char *rhs);
void do_multiply(longint_t *var1, longint_t *var2);
void do_power(longint_t *var1, longint_t *var2);
void do_divide(longint_t *var1, longint_t *var2);
void swap_longint (longint_t *var1, longint_t *var2);
longint_t longint_zero (void);


/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
    char line[LINELEN+1] = {0};
    longint_t vars[NVARS];

    zero_vars(vars);
    print_prompt();
    while (read_line(line, LINELEN)) {
        if (strlen(line) > 0) {
            /* non empty line, so process it */
            process_line(vars, line);
        }
        print_prompt();
    }

    print_tadaa();
    return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
    can be confirmed that the input is coming from a terminal.
    Plus, output might be going to a file, that's why the prompt,
    if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
    if (isatty(fileno(stdin))) {
        fprintf(stderr, "> ");
        fflush(stderr);
    }
}

void
print_tadaa() {
    /* all done, so pack up bat and ball and head home,
        getting the exact final lines right is a bit tedious,
        because input might be coming from a file and output
        might be going to a file 
    */
    if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
        printf("\n");
    }
    printf("ta daa!!!\n");
    if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
        fprintf(stderr, "\n");
    }
}

void
print_error(char *message) {
    /* need to write an error message to the right place(s)
    */
    if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
        fprintf(stderr, "%s\n", message);
        fflush(stderr);
    }
    if (!isatty(fileno(stdout))) {
        printf("%s\n", message);
    }
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
    returns false if there is no input available.
    All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
    int i=0, c;
    while (((c=getchar())!=EOF) && (c!='\n')) {
        if (i<maxlen && !isspace(c)) {
            line[i++] = c;
        }
    }
    line[i] = '\0';
    /* then, if the input is coming from a file or the output
        is going to a file, it is helpful to echo the input line
        and record what the command was 
    */
    if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
        printf("%s%s\n", PROMPT, line);
    }
    return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(longint_t vars[], char *line) {
    int varnum, optype, status;
    longint_t second_value = longint_zero();

    /* determine the LHS variable, it
        must be first character in compacted line
    */
    varnum = to_varnum(line[0]);
    if (varnum==ERROR) {
        print_error("invalid LHS variable");
        return;
    }

    /* more testing for validity 
    */
    if (strlen(line)<2) {
        print_error("no operator supplied");
        return;
    }

    /* determine the operation to be performed, it
        must be second character of compacted line
    */
    optype = line[1];
    if (strchr(ALLOPS, optype) == NULL) {
        print_error("unknown operator\n");
        return;
    }

    /* determine the RHS argument (if one is required),
        it must start in the third character of compacted line
    */
    if (optype != PRINT) {
        if (strlen(line)<3) {
            print_error("no RHS supplied");
            return;
        }
        status = get_second_value(vars, line+2, &second_value);
        if (status==ERROR) {
            print_error("RHS argument is invalid");
            return;
        }
    }

    /* finally, do the actual operation
    */
    if (optype == PRINT) {
        do_print(varnum, vars+varnum);
    } else if (optype == ASSIGN) {
        do_assign(vars+varnum, &second_value);
    } else if (optype == PLUS) {
        do_plus(vars+varnum, &second_value);
    // you will need to add further operators here
    } else if (optype == MULT) {
        do_multiply(vars+varnum, &second_value);
    } else if (optype == POWR) {
        do_power(vars+varnum, &second_value);
    } else if (optype == DIVS) {
        do_divide(vars+varnum, &second_value);  
    } else {
        printf("operation not available\n");
        return;
    }

    return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
    int varnum;
    varnum = ident - CH_A;
    if (0<=varnum && varnum<NVARS) {
        return varnum;
    } else {
        return ERROR;
    }
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(longint_t vars[], char *rhsarg,
            longint_t *second_value) {
    char *p;
    int varnum2;
    if (isdigit(*rhsarg)) {
        /* first character is a digit, so RHS is a number
            now check the rest of RHS for validity 
        */
        for (p=rhsarg+1; *p; p++) {
            if (!isdigit(*p)) {
                // nope, found an illegal character 
                return ERROR;
            }
        }
        // nothing wrong, ok to convert
        *second_value = parse_num(rhsarg);
        return !ERROR;
    } else {
        // argument is not a number, so should be a variable
        varnum2 = to_varnum(*rhsarg);
        if (varnum2==ERROR || strlen(rhsarg)!=1) {
            // nope, not a variable either
            return ERROR;
        }
        // and finally, get that variable's value
        do_assign(second_value, vars+varnum2);
        return !ERROR;
    }
    return ERROR;
}

/* Set the vars array to all zero values
*/
void
zero_vars(longint_t vars[]) {
    int i;
    longint_t zero = longint_zero();
    for (i=0; i<NVARS; i++) {
        do_assign(vars+i, &zero);
    }
    return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a string
*/
longint_t
parse_num(char *rhs) {
    longint_t number = longint_zero();

    // Allocate buddy variable; number of digits
    number.length = strlen(rhs);

    // Raise error if input is more than 500 digits, if no leading zeroes input
    if (number.length > INTSIZE) {
        printf("Integer Overflow: Input is more than 500 digits\n");
        exit(EXIT_FAILURE);
    }

    // Allocate each digit as an int
    int start = 0;
    for (int i = 0, n = number.length - 1; i <= n; i++) {
        // Convert each char digit into an int
        int digit = rhs[i] - ASCII_ZERO;
        
        // Identify leading 0s and adjust buddy variable accordingly
        if (digit > 0) {
            start = 1;
        } else if ((start == 0) && (digit == 0)) {
            number.length -= 1;
        }
        
        // Start allocating digits in reverse after the leading 0s, if any
        if (start) {
            number.digit[n - i] = digit;
        }
    }

    // If value is zero
    if (!start) {
        number.length = 1;
        number.digit[0] = 0;
    }

    return number;
}

/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, longint_t *var) {
    printf("register %c: ", varnum+CH_A);

    // Determine number of digits plus commas
    int len_comma = var->length + (var->length / PUT_COMMAS);

    // Remove one comma if digits can be grouped by 3
    if ((var->length % PUT_COMMAS) == 0) {
        len_comma -= 1;
    }

    int comma = 0, counter = len_comma - NULL_ALLOC;
    char to_print[len_comma + NULL_ALLOC];

    // Reverse digits back in order
    for (int i = 0; i < var->length; i++) {
        to_print[counter] = var->digit[i] + CH_ZERO;
        comma++;
        counter--;

        // Place commas every 3 digits
        if (comma == PUT_COMMAS) {
            to_print[counter] = CH_COM;
            counter--;
            comma = 0;
        }
    }

    // Add null character
    to_print[len_comma] = '\0';

    printf("%s\n", to_print);
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2
*/
void
do_assign(longint_t *var1, longint_t *var2) {
    *var1 = *var2;
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(longint_t *var1, longint_t *var2) {
    int max_len = 0, carry = 0;

    // Determine the number with most digits
    if (var1->length >= var2->length) {
        max_len = var1->length;
    } else {
        max_len = var2->length;
    }

    // Loop through digits and add carryover values
    for (int i = 0; i < max_len; i++) {
        var1->digit[i] += var2->digit[i];
        var1->digit[i] += carry;
        carry = var1->digit[i] / INT_TEN;

        if (var1->digit[i] > 9) {
            // Show error message and exit if more than 500 digits after adding
            if ((carry > 0) && (i == (INTSIZE - 1))) {
                printf("Integer Overflow: Result is more than 500 digits\n");
                exit(EXIT_FAILURE);
            }

            var1->digit[i] %= INT_TEN;
        } 
    }

    // Add digit if there is carryover and update buddy variable
    if (carry > 0) {
        var1->digit[max_len] += carry;
        var1->length = max_len + 1;  
    } else {
        var1->length = max_len;  
    }
}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/

/* Update the indicated variable var1 by doing a multiplication
   using var2 to compute var1 = var1 * var2
*/
void
do_multiply(longint_t *var1, longint_t *var2) {
    // Store value of var2 in case of any changes
    longint_t temp = longint_zero();
    do_assign(&temp, var2);

    // Determine the result's highest possible length in digits
    int product_len = var1->length + var2->length;
    int carry = 0, less_len = 0;

    // Throw error if result will be above 500 digits
    if (product_len == (INTSIZE + 1)) {
        product_len = INTSIZE;
    } else if (product_len > (INTSIZE + 1)) {
        printf("Integer Overflow: Result is more than 500 digits\n");
        exit(EXIT_FAILURE);
    }

    // Determine the number with lesser digits
    if (var1->length >= var2->length) {
        less_len = var2->length;
        swap_longint(var1, var2);
    } else {
        less_len = var1->length;
    }

    // Create an array of the result of each row of multiplication
    longint_t result[INTSIZE];
    zero_vars(result);

    // Carry out multiplication, assigning carry variables accordingly
    for (int i = 0; i < less_len; i++) {
        carry = 0;
        for (int j = 0; j < var2->length; j++) {
            int product = var1->digit[i] * var2->digit[j];
            result[i].digit[j + i] = (product + carry) % INT_TEN;
            carry = (product + carry) / INT_TEN;
        }

        // If there is carry at the end, adds to the front of the number
        if ((carry > 0) && ((var2->length + i) < INTSIZE)) {
            result[i].digit[var2->length + i] = carry;
        } else if ((carry > 0) && ((var2->length + i) >= INTSIZE)) {
            printf("Integer Overflow: Result is more than 500 digits\n");
            exit(EXIT_FAILURE);
        }
    }

    // Determine the length in digits of the results without leading 0s
    for (int i = 0; i < less_len; i++) {
        for (int j = product_len; j > 0; j--) {
            if (result[i].digit[j - 1] != 0) {
                result[i].length = j;
                break;
            } else if ((j - 1) == 0) {
                result[i].length = 1;
            }
        }
    }

    // Sum the multiplication results of each row
    for (int i = 1; i < less_len; i++) {
        do_plus(&result[0], &result[i]);
    }

    // Assign the multiplication result to var1
    do_assign(var1, &result[0]);

    // Ensure that value of var2 stays the same
    do_assign(var2, &temp);
}

/****************************************************************/

/* Update the indicated variable var1 by doing an exponentiation
   using var2 to compute var1 = var1 ^ var2
*/
void
do_power(longint_t *var1, longint_t *var2) {
    // Powered by 0 or 1 powered by anything both equals to 1
    if (((var2->length < 2) && (var2->digit[0] == 0)) || 
        ((var1->length < 2) && (var1->digit[0] == 1))) {
        longint_t zero = longint_zero();
        zero.length = 1;
        zero.digit[0] = 1;
        do_assign(var1, &zero);
        return;
    }

    longint_t temp = longint_zero();
    do_assign(&temp, var1);

    // Carry out exponentiation through repeated multiplication
    for (int i = 0; i < var2->length; i++) {
        double num = (var2->digit[i] * pow(INT_TEN, i)) - 1;
        for (int j = 0; j < num; j++) {
            do_multiply(var1, &temp);
        }
    }

    // Accounts for the missed multiplication when switching digits
    for (int i = 0; i < (var2->length - 1); i++) {
        if (var2->digit[i] != 0) {
            do_multiply(var1, &temp);
        }
    }
}

/****************************************************************/

/* Update the indicated variable var1 by doing a division
   using var2 to compute var1 = var1 / var2
*/
void
do_divide(longint_t *var1, longint_t *var2) {
    // Divide by zero not allowed
    if ((var2->length < 2) && (var2->digit[0] == 0)) {
        printf("Invalid Operation: Divide by 0\n");
        exit(EXIT_FAILURE);
    } 
}

/****************************************************************/

/* Swaps the values of two longint_t variables
*/
void 
swap_longint (longint_t *var1, longint_t *var2) {
    longint_t temp;
    do_assign(&temp, var2);
    do_assign(var2, var1);
    do_assign(var1, &temp);
}

/****************************************************************/

/* Generate a longint with the value of zero, which is one digit long
*/
longint_t
longint_zero (void) {
    longint_t zero;
    zero.length = LEN_ONE;
    for (int i = 0; i < INTSIZE; i++) {
        zero.digit[i] = 0;
    }

    return zero;
}