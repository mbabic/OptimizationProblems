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

static int
calculate_initial_solution_cost(Node *, int *, int *, int);

static int
calculate_proposed_solution_cost(Graph *, Node *);

static void
update_color_classes(Graph *g, Node *, int *, int, int);

static void
update_bad_edges(Graph *g, Node *, int *, int, int); 

static void
generate_color_classes(Node *, int **, int);

static void
generate_bad_edges(Graph *, Node *, int **);

static void
update_pqueue_priorities(Graph *, PQueue *);

char *
solve_coloring_instance(Graph *g) {

        Node *S, *S_proposed, *S_opt;
        int *C;                         /* An array such that C[i] is the size
                                           of the i_th color class. */
        int *E;                         /* An array such that E[i] is the 
                                           number of "bad edges" in the i_th
                                           color class (i.e., the number of 
                                           edges whose endpoints are colored
                                           the same color. */
        int c, c_proposed, c_opt;


        assert(g != NULL);

        /* Produce initial solution using DSATUR algorithm. */
        produce_initial_solution(g);        
        
        /* Copy initial solution into S and S_opt. */
        graph_copy_nodes(g, &S);
        graph_copy_nodes(g, &S_opt);



        /* Init auxilliary structs. */
        generate_color_classes(S, &C, g->n);
        generate_bad_edges(g, S, &E);
        
        
        return NULL;
}

/**
 * Calculate the cost of the initial solution to the coloring problem.
 * The "cost"/"target" function is given by the following expression:
 *
 * f(S) = sum_{i=1}^{k} (2*|C_i|*|E_i| - |C_i|^2)
 *
 * where 
 *      k = number of color classes
 *
 * TODO: CITE JOHNSON PAPER
 */
static int
calculate_initial_solution_cost(Node *S, int *C, int *E, int n) {

        int i, cost = 0;

        assert(S != NULL);
        assert(C != NULL);
        assert(E != NULL);
        assert(n > 0);

        for (i = 0; i < n; i++) {
                cost += (2 * E[i] * C[i]) - ((C[i])^2);
        }

        return cost;
}


static void
generate_bad_edges(Graph *g, Node *S, int **E) {

        assert(g != NULL);
        assert(S != NULL);
        assert(E != NULL);
        assert(*E != NULL);

        *E = calloc(g->n, sizeof(int));
        if (!*E) ALLOCATION_ERROR();

        /* Since we know initial solution is a correct coloring, 
         * for all i |E_i| = 0 */
}

/**
 * Initialize the structure containing the sizes of each color class.
 * @param Node *S
 *      Pointer to array of nodes (i.e., the solution from which the color
 *      class sizes are to be determined)
 * @param int **C
 *      Pointer to array of integers used to store the color class size
 *      structure.
 * @param int n
 *      The number of nodes in the graph/solution.
 */
static void
generate_color_classes(Node *S, int **C, int n) {

        int i;

        assert(S != NULL);
        assert(C != NULL);
        assert(*C != NULL);
        assert(n > 0);

        *C = calloc(n, sizeof(int));
        if (!*C) ALLOCATION_ERROR();

        for (i = 0; i < n; i++) {
                (*C)[S[i].color]++;
        } 
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
        Node *u;
        double pri;
        int c;

        pq = pqueue_init(g->n, node_calculate_priority);        

        update_pqueue_priorities(g, pq); 

        pqueue_dequeue(pq, (void **) &u, NULL);

        u->color = 1;

        graph_update_saturation_degrees(g, u);
        update_pqueue_priorities(g, pq);

        while (!pqueue_is_empty(pq)) {

                pqueue_dequeue(pq, (void **) &u, &pri);
               
                c = graph_get_lowest_available_color(g, u);

                u->color = c;
               
                graph_update_saturation_degrees(g, u);
                update_pqueue_priorities(g, pq);
        }
#ifdef DEBUG
        /*
         * If debug flag defined, validate results of output. 
         * (i.e., ensure that the coloring produced is valid)
         */
        graph_is_valid_coloring(g, 1);

#endif
}

static void
update_pqueue_priorities(Graph *g, PQueue *pq) {

        Node *n;
        int i;

        assert(g != NULL);
        assert(pq != NULL);

        pqueue_reset(pq);

        for (i = 0; i < g->n; i++) {
                n = &(g->nodes[i]);
                if (!n->color) pqueue_enqueue(pq, (void *) n);
        }
}
