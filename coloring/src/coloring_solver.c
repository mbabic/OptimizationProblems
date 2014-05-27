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
update_pqueue_priorities(Graph *, PQueue *);

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

        PQueue *pq;
        Node *u, *v;
        int c;

        pq = pqueue_init(g->n, node_calculate_priority);        

        update_pqueue_priorities(g, pq); 

        pqueue_dequeue(pw, &u, NULL);

        u->color = 1;

        graph_update_saturation_degrees(g);
        update_pqueue_priorities(g, pq);

        while (!pqueue_is_empty(pq)) {

                pqueue_dequeue(pw, &u, NULL);
                
                c = graph_get_lowest_available_color(g, u);

                u->color = c;
                
                graph_update_saturation_degrees(g);
                update_pqueue_priorities(g, pq);
        }
}

static void
update_pqueue_priorities(Graph *g, PQueue *pq) {

        Node *n;
        int i;

        assert(g != NULL);
        assert(pq != NULL);

        pqueue_reset(pq);

        for (i = 0; i < g->n; i++) {
                n = &(g->elements[i]);
                if (!n->color) pqueue_enqueue(pq, (void *) n);
        }
}
