/**
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "utils.h"

#define MAX_LINE_LENGTH 128
#define NARGS 2

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

static void
parse_args(int, char **, Graph **);

int
main(int argc, char **argv) {

        Graph *g;

        parse_args(argc, argv, &g);


        return 0;
}


/**
 * Parse command line args and initialize Graph instance to be used by
 * the solver.
 * @param int argc
 *      argc as passed at program execution.
 * @param char **argv
 *      argv as passed at program execution.
 * @param Graph **g
 *      Address of pointer to graph structure to be initialized.
 */
static void
parse_args(int argc, char **argv, Graph **g) {

        char buf[MAX_LINE_LENGTH];
        char delimiters[] = " \n";
        char *token, *err;
        FILE *in;
        int lineno = 0, u, v, n;

        if (argc != NARGS) {
                usage();
        }

        in = fopen(argv[1], "r");
        if (!in) {
                fprintf(stderr, "Input file %s could not be opened.\n",
                                argv[1]);
                exit(1);
        }

        memset(buf, '\0', MAX_LINE_LENGTH);


        while (fgets(buf, MAX_LINE_LENGTH, in) != NULL) {
                
                if (lineno == 0) {
                        
                        token = strtok(buf, delimiters);
                        n = (int) strtol(token, &err, 10);

                        if (err[0] != '\0' || n <= 0) format_error(lineno);

                        // TODO: we ignore number of edges in input

                        *g = graph_init(n);
                
                } else {
                        /* Get endpoints of edge being define. */
                        token = strtok(buf, delimiters);
                        u = (int) strtol(token, &err, 10);                  

                        if (err[0] != '\0') format_error(lineno);

                        token = strtok(NULL, delimiters);
                        v = (int) strtol(token, &err, 10);
                        
                        if (err[0] != '\0') format_error(lineno);

                        graph_add_edge(*g, u, v);

                        DEBUG_PRINT("Added edge between %d and %d.\n", u, v);
                }
                lineno++;
        }


}


