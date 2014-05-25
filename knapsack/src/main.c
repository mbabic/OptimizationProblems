#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"
#include "solver.h"
#include "utils.h"

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

        Item *items;    /* Array of Item structs defining problem instance. */
        char *sol;      /* Solution string returned by solver. */
        int n,          /* The number of items in the knapsack. */ 
            K;          /* The capacity of the knapsack. */

        parse_args(argc, argv, &n, &K, &items);        

        sol = solve_knapsack_instance(n, K, items);

        printf("%s", sol);

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
        int lineno = 0, weight, value;

        if (argc < NARGS) {
                usage();
        }

        in = fopen(argv[1], "r");
        if (in == NULL) {
                fprintf(stderr, "Input file: %s could not be found.\n", 
                                argv[1]);
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

                        if (err[0] != '\0') {
                                format_error(lineno); 
                        } 

                        if (*n < 0) {
                                fprintf(stderr,
                                        "Number of items in knapsack cannot "
                                        "be < 0.\n");
                                exit(1);
                        }

                        /* Get K from input file. */
                        token = strtok(NULL, delimiters);
                        *K = (int) strtol(token, &err, 10);
                        
                        if (err[0] != '\0') {
                              format_error(lineno);
                        }  

                        if (*K < 0) {
                                fprintf(stderr,
                                        "Knapsack capacity cannot be < 0.\n");
                                exit(1);
                        }

                        /* Allocate memory for array of items. */
                        *items = malloc(sizeof(Item) * (*n));
                        if (*items == NULL) {
                                fprintf(stderr, 
                                        "Failed to allocate memory for items "
                                        "array.");
                        }

                        DEBUG_PRINT("n = %d\n K = %d\n", *n, *K);

                } else {
                        /* Instantiate no more than n items. */
                        if (lineno > *n) break;

                        /* Get item weight from input file. */
                        /* TODO: determine if weight is allowed to be < 0. */
                        token = strtok(buf, delimiters);
                        weight = (int) strtol(token, &err, 10);

                        if (err[0] != '\0') {
                                format_error(lineno);
                        } 
        
                        /* Get item value from input file. */
                        /* TODO: determine if value is allowed to be < 0. */
                        token = strtok(NULL, delimiters);
                        value = (int) strtol(token, &err, 10);
                        
                        if (err[0] != '\0') {
                                format_error(lineno);
                        }

                        (*items)[lineno - 1].isTaken = 0;
                        (*items)[lineno - 1].weight = weight;
                        (*items)[lineno - 1].value = value;

                        DEBUG_PRINT("items[%d].weight = %d\n items[%d].value "
                                        "= %d\n", lineno - 1, weight, 
                                        lineno - 1, value);
                }
                lineno++;
        }

        if (lineno < (*n) + 1) {
                fprintf(stderr, "Less items were given than was specified "
                                "on first line of input.\n");
                exit(1);
        }
}
