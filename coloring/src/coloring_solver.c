/**     
 * Uses the DSATUR algorith and simulating annealing thereafter to produce a
 * solution to the coloring problem given an instance of a graph.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graph.h"
#include "pqueue.h"
#include "utils.h"

/* Define simulation constants. */
/* TODO: determine good values for constants below from paper. */
#define FREEZE_LIM 1
#define INITIAL_TEMPERATURE 0.8
#define TEMPFACTOR 0.5
#define MINPERCENT 0.1
#define SIZEFACTOR 10
#define N 9.5
#define CUTOFF 10

static void
produce_initial_solution(Graph *);

static int
calculate_initial_solution_cost(Node *, int *, int *, int);

static int
calculate_proposed_solution_cost(Graph *, Node *, int *, int *, int, int, int);

static void
generate_color_classes(Node *, int **, int);

static void
generate_bad_edges(Graph *, Node *, int **);

static void
propose_new_solution(Graph *, int, int *, int *);

static void
update_pqueue_priorities(Graph *, PQueue *);

static void
update_color_classes(int *, int, int, int);

static void
update_bad_edges(Graph *g, Node *, int *, int, int, int); 

char *
solve_coloring_instance(Graph *g) {

        Node *S, *S_opt;
        int *C;                         /* An array such that C[i] is the size
                                           of the i_th color class. */
        int *E;                         /* An array such that E[i] is the 
                                           number of "bad edges" in the i_th
                                           color class (i.e., the number of 
                                           edges whose endpoints are colored
                                           the same color. */
        int c, c_proposed, c_opt, freeze_count = 0, n_trials = 0, changes = 0,
            proposed_color, proposed_node, old_color, delta, max_colors, i;

        assert(g != NULL);

        /* Seed randomon number generator. */
        srand(time(NULL));

        /* Produce initial solution using DSATUR algorithm. */
        produce_initial_solution(g);        
        
        /* Copy initial solution into S and S_opt. */
        graph_copy_nodes(g, &S);
        graph_copy_nodes(g, &S_opt);

        /* Init auxilliary structs. */
        generate_color_classes(S, &C, g->n);
        generate_bad_edges(g, S, &E);
       
        /* 
         * Consider the colors used in the initial solution to be the
         * max number of colors from which we can choose in our random
         * solutions in the annealing process.
         */
        for (i = 1; i < g->n; i++) {
                if (C[i] == 0) max_colors = i-1; 
        }


        c = calculate_initial_solution_cost(S, C, E, g->n);
        c_opt = c;

        while (freeze_count < FREEZE_LIM) {
        
                n_trials = 0;

                while (n_trials < SIZEFACTOR * N && changes < CUTOFF * N) {

                        n_trials++;
                        
                        propose_new_solution(g, max_colors, &proposed_node, 
                            &proposed_color);
                        
                        c_proposed = calculate_proposed_solution_cost(g, S,
                            C, E, c, proposed_node, proposed_color);

                        delta = c_proposed - c;

                        if (delta <= 0) {
                                /* New solution better than previous. */
                                changes++;
                                c = c_proposed;
                                old_color = S[proposed_node].color;
                                update_color_classes(C, proposed_node, 
                                    old_color, proposed_color);                               
                                update_bad_edges(g, S, E, proposed_node,
                                    old_color, proposed_color);
                                S[proposed_node].color = proposed_color; 
                        }

                }
        }


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

/**
 * Calculate the cost of the proposed solution without committing to updating
 * the color class and bad edge data structures.
 * TODO: mixing british and american spellings in neighbour_color variables.
 * Oh well.
 * @param Graph *g
 *      Pointer to problem graph instance.
 * @param Node *S
 *      The old solution we are improving upon.
 * @param int *C
 *      Color classes data structure.
 * @param int *E
 *      Bad edges data structure.
 * @param int neighbour_cost
 *      The cost of the solution to which the proposed solution is a 
 *      neighbour.
 * @param int u
 *      Id of node whose color is to be changed.
 * @param int new_color
 *      The new color we are proposing to change the node 'u' to.
 */
static int
calculate_proposed_solution_cost(Graph *g, Node *S, int *C, int *E, 
    int neighbour_cost, int u, int new_color) {
        
        int i = 0, cost = 0, neighbour_color, C_new_color, E_new_color,
            C_neighbour_color, E_neighbour_color, n_conflicts_resolved = 0,
            n_conflicts_introduced = 0;

        cost = neighbour_cost;

        neighbour_color = S[u].color;

        /* 
         * Subtract from cost the cost related to the color classes are being
         * proposed to be updated.
         */
        cost -= (2 * C[neighbour_color] * E[neighbour_color]) - 
                ((C[neighbour_color])^2);

        cost -= (2 * C[new_color] * E[new_color]) - ((C[new_color])^2);

        /*
         * Calculate updated values of relevants color class and bad edge
         * data structure members if we were to implement the proposed 
         * solution.
         */
        C_new_color = C[new_color] + 1;
        C_neighbour_color = C[neighbour_color] - 1;

        /*
         * Intuition for calculating value of E_new_color and
         * E_neighbour_color: changing node u to new_color can only the number
         * of bad edges in color class neighbour_color and increase the number
         * of bad edges in color class new_color.
         */
        for (i = 0; i < g->n; i++) {

                if (g->adj[u][i] == 1 && i != u) {

                        if (S[i].color == neighbour_color) {
                                n_conflicts_resolved++;
                        } else if (S[i].color == new_color) {
                                n_conflicts_introduced++;
                        }
                }
        }

        E_new_color = E[new_color] + n_conflicts_introduced;
        E_neighbour_color = E[neighbour_color] - n_conflicts_resolved;

        /* We can now calculate the updated cost. */
        cost += (2 * C_neighbour_color * E_neighbour_color) -
                ((C_neighbour_color)^2);

        cost += (2 * C_new_color * E_new_color) -
                ((C_new_color)^2);

        return cost;
}

static void
generate_bad_edges(Graph *g, Node *S, int **E) {

        assert(g != NULL);
        assert(S != NULL);
        assert(E != NULL);
        assert(*E != NULL);

        *E = calloc(g->n + 1, sizeof(int));
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

        *C = calloc(n + 1, sizeof(int));
        if (!*C) ALLOCATION_ERROR();

        for (i = 0; i < n; i++) {
                (*C)[S[i].color]++;
        } 
}

static void
propose_new_solution(Graph *g, int max_colors, int *proposed_node, 
    int *proposed_color) {

        *proposed_node = rand() % g->n;
        *proposed_color = rand() % max_colors;
}

/**
 * Update the color classes data structure when a better solution has been
 * proposed.
 * @param int *C
 *      The color classes data structure.
 * @param int u
 *      The id of the node whose color was changed.
 * @param old_color
 *      The old color of the node (i.e., the color of the node in the solution
 *      that is being replaced)
 * @param new_color
 *      The new color for the node 'u'
 */
static void
update_color_classes(int *C, int u, int old_color, int new_color) {
        C[old_color]--;
        C[new_color]++;
}

/**
 * Update the bad edges data structure.
 * NOTE: relies on the old solution's array of nodes to make proper updates.
 * @param Graph *g
 *      The instance graph for the coloring problem.
 * @param Node *S
 *      The old solution (i.e., the one that is being replaced that prompted
 *      the update)
 * @param int *E
 *      The bad edges data structure.
 * @param int u
 *      The id of the node whose color was changed.
 * @param old_color
 *      The old color of the node (i.e., the color of the node in the solution
 *      that is being replaced)
 * @param new_color
 *      The new color for the node 'u'
 */
static void
update_bad_edges(Graph *g, Node *S, int *E, int u, int old_color, int new_color) {

        int i;
        for (i = 0; i < g->n; i++) {
        
                if (g->adj[u][i] == 1 && u != i) {

                        if (S[i].color == old_color) {
                                E[old_color]--;
                        } else if (S[i].color == new_color) {
                                E[new_color]++;
                        }
                }
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
