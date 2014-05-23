#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/*
 * Function signature definitions.
 */
void
parse_args(int, char **, int *, int *, Item **);

/**
 * Prints usage message on passing of bad cmd line args.
 */
static void
usage() {
        extern char * __progname;
        fprintf(stderr, "Usage: ./%s { path to input file }\n", __progname);
        exit(1);
}

/**
 * Prints message informing user that data is not in expected format.
 */
void
format_error(int lineno) {
        fprintf(stderr, "Input data not in expected format on line %d\n",
                        lineno);
        exit(1);
}

int 
main(int argc, char **argv) {

        Item *items;
        int n,          /* The number of items in the knapsack. */ 
            K;          /* The capacity of the knapsack. */

        parse_args(argc, argv, &n, &K, &items);        

        /* TODO: call solver. */


        /* TODO: write solver output in the format expected by solver.py. */

        return 0;
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
void
parse_args(int argc, char **argv,int *n, int *K, Item **items) {

        char buf[MAX_LINE_LENGTH];
        char *token, *err;
        char delimiters[] = " \n";
        FILE *in;
        int line_length, lineno = 0;

        if (argc < NARGS) {
                usage();
        }

        in = fopen(argv[2], "r");
        if (in == NULL) {
                fprintf(stderr, "Input file: %s could not be found.", argv[2]);
                exit(1);
        }

        memset(buf, '\0', MAX_LINE_LENGTH);
        while (fgets(buf, MAX_LINE_LENGTH, in) != NULL) {

                if (lineno == 0) {
                        /*
                         * We expect first line to contain n and K separated
                         * by a single space.
                         */
                        
                        /* Get n from input file. */
                        token = strtok(buf, delimiters);
                        *n = (int) strtol(token, &err, 10);

                        if (err[0] == '\0') {
                                format_error(lineno); 
                        } 

                        /* Get K from input file. */
                        token = strtok(NULL, delimiters);
                        *K = (int) strtol(token, &err, 10);
                        
                        if (err[0] != '\0') {
                              format_error(lineno);
                        }  
                } else {
                        
                }
                
                lineno++;
        }
}
