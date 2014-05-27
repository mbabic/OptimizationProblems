/**
 * Uses the DSATUR algorith and simulating annealing thereafter to produce a
 * solution to the coloring problem given an instance of a graph.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "pqueue.h"
#include "utils.h"

static void
produce_initial_solution(Graph *);

static void
update_queue_keys(Graph *, PQueue *);

char *
solve_coloring_instance(Graph *g) {
        
        produce_initial_solution(g);        


        return NULL;
}

/**
 * Produces an initial solution to the coloring problem using the DSATUR
 * algorithm.
 * @param Graph *g
 *      Pointer to the graph specifying the instance of the problem to be
 *      solved.
 */
static void
produce_initial_solution(Graph *g) {

        

}

