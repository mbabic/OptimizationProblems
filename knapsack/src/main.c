#include <stdio.h>

#include "item.h"

/*
 * Define debug printing macro.
 */
#ifdef DEBUG
#define DEBUG_PRINT(s) do { printf s ; }  while (0)
#else
#define DEBUG_PRINT(s) do {} while (0)
#endif


/*
 * Define max length of line we will read from input file.
 */
#define MAX_LINE_LENGTH 128

/* 
 * Define number of arguments expected from cmd line.
 */
#define NARGS 2

/**
 * Prints usage message on passing of bad cmd line args.
 */
static void
usage() {
        exit(1, "Usage: ./knapsack_solver -i { path to input file }");
}

int 
main(int argc, char **argv) {

        Item *items;
        int n,          /* The number of items in the knapsack. */ 
            K;          /* The capacity of the knapsack. */

        parse_args(argc, argv, &n, &K, &items);        

        /* TODO: call solver. */


        /* TODO: write solver output in the format expected by solver.py. */
}

/**
 * Parse command line args and initialize Item structs to be used by
 * the solver.
 * @param int argc
 *      argc as passed at program execution.
 * @param char **argv
 *      argv as passed at program execution.
 * @param int *n
 *      Pointer to integer variable into which the number of items in the
 *      knapsack will be stored.
 * @param int *K
 *      Pointer to integer variable into which the capacity of the knapsack
 *      will be stored.
 * @param Item **items
 *      Pointer to array of Items.  Function will initialize needed heap memory.
 */
parse_args(int argc, char **argv,int *n, int *K, Item **items) {

        char buf[MAX_LINE_LENGTH];
        char *token;
        FILE *in;
        int line_length, lineno = 0;

        if (argc < NARGS) {
                usage();
        }

        in = fopen(argv[2], "r");
        if (in == NULL) {
                exit(1, "Input file provided could not be opened.");
        }

        memset(buf, '\0', MAX_LINE_LENGTH);
        while (fgets(buf, MAX_LINE_LENGTH, in) != NULL) {

                if (lineno == 0) {
                        /*
                         * We expect first line to contain n and K separated
                         * by a single space.
                         */
                        
                        /* Get n from input. */
                        token = strtok(buf, ' ');

                        // if ((n = strtol(token, (char **) NULL, 10))    
                
                }
                
                lineno++;
        }
}
