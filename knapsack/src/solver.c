/*
 * Module implementating functionality with regarding to solving an instance
 * of the knapsack problem.
 * Marko Tomislav Babic - mbabic@ualberta.ca
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "item.h"
#include "pqueue.h"
#include "solver.h"
#include "utils.h"

/**
 * Prints message indicating memory allocation failure and exits program.
 */
static void
allocation_error() {
        extern char *__progname;
        fprintf(stderr, "Memory allocation failed in program %s.  Exiting.\n",
                __progname);
        exit(1);
}

static double
bound(int, int, Item *, Node *);

static void
construct_solution(int **, int, int, Item *);

static char *
construct_solution_string(int **, int, int, Item *);

static char *
solve_knapsack_instance_bb(int, int, Item *);

static char *
solve_knapsack_instance_dp(int, int, Item *);



/**
 * Solve instance of knapsack problem parameterized by given arguments.
 * @param int n
 *      The number of items to be considered.
 * @param int K
 *      The capacity of the knapsack.
 * @param Item *items
 *      Array of Item structs corresponding to the items to be placed in the
 *      knapsack. 
 *
 * @return
 *      String encoding solution.
 */
char *
solve_knapsack_instance(int n, int K, Item *items) {
        return solve_knapsack_instance_bb(n, K, items);
}

/**
 * Solve given instance of knapsack problem using a dynamic programming
 * approach
 */
char *
solve_knapsack_instance_dp(int n, int K, Item *items) {
        /*
         * Will implement dynamic programming solution according to the 
         * recursive relationship:
         * A[i, w] = max(A[i-1, w], v_i + A[i=1, w-w_i])
         */
        Item item;
        int **A, i, w; 

        /* 
         * Init solution matrix and auxilliary boolean matrix used in 
         * solution reconstruction. 
         */
        A = malloc((n + 1) * sizeof(int *));
        if (!A) allocation_error(); 

        for (i = 0; i < (n + 1); i++) {
                A[i] = malloc((K+1) * sizeof(int));
                if (!A[i]) allocation_error(); 
        }

        /* Set initial values. */
        for (w = 0; w <= K; w++) {
               A[0][w] = 0; 
        }

        /* Populate matrix of sub-solutions. */
        for (i = 1; i < (n + 1); i++) {
                item = items[i-1];
                for (w = 0; w <= K; w++) {
                        if (item.weight <= w) {
                                A[i][w] = MAX(A[i-1][w],
                                                A[i-1][w-item.weight] +
                                                item.value);
                        } else {
                                A[i][w] = A[i-1][w];
                        }
                }
        }

        /* Construct solution from values in matrix of sub-solutions. */
        construct_solution(A, n, K, items);

        DEBUG_PRINT("Solution: %d\n", A[n][K]);


#ifdef DEBUG
        // Run validation test if is debug build.
        int value_sum = 0, weight_sum = 0;
        for (i = 0; i < n; i++) {
                if (items[i].isTaken) {
                        value_sum += items[i].value;
                        weight_sum += items[i].weight;
                }
        }

        assert(value_sum == A[n][K] && "Sum of values of items in knapsack"
                                       "should match value of final solution.");
        assert(weight_sum <= K && "Sum of weights of items in knapsack should"
                                  "be less than capacity of knapsack.");
#endif

        return construct_solution_string(A, n, K, items);

} 

/**
 * For each item, set its isTaken variable based on the values in the 
 * sub-solution matrix.  That is, after having tabulated all entries in the
 * matrix, construct_solution uses the values to determine which items
 * were taken and which were not. 
 */
static void
construct_solution(int **A, int n, int K, Item *items) {
        int w = K, i = n;
        while (i > 0 && w > 0) {
                if (A[i][w] != A[i-1][w]) {
                        // Item i appeared in the solution.  Set its isTaken
                        // attribute to be true.
                        items[i-1].isTaken = 1;
                        w -= items[i-1].weight;
                        DEBUG_PRINT("Solution: item %d isTaken = %d\n",
                                i-1, items[i-1].isTaken);
                } 
                i--;
        } 
}

static char *
construct_solution_string(int **A, int n, int K, Item *items) {

        char *sol, *is_taken_str;
        int len, i;
        
        /* Calculate space required to create string. */
        /* 
         * For first line, need (MAX LENGTH IN DIGITS OF INTEGER) + 
         * 4 bytes (3 whitespace bytes, 1 byte for optimality boolean)
         */
        len = 9 + 4;

        /* 
         * For second line, need 2*n bytes (1 byte for each boolean indicating
         * whether the i_th item was taken, n bytes for whitepsace.
         */
        len += 2*n; 

        sol = malloc((len + 1) * sizeof(char));
        if (!sol) allocation_error();
        
        is_taken_str = malloc((2*n + 1) * sizeof(char));
        if (!is_taken_str) allocation_error();

        sprintf(sol, "%d %d\n", A[n][K], 1);

        for (i = 0; i < 2*n; i += 2) {
                is_taken_str[i] = items[i / 2].isTaken ? '1' : '0';
                is_taken_str[i+1] = ' ';
        }
        is_taken_str[2*n] = '\n';
        is_taken_str[(2*n) + 1] = '\0';

        strcat(sol, is_taken_str);
        
        DEBUG_PRINT("Solution string: %s\n", sol);
       
        return sol; 
}

/**
 * Solve given instance of the knapsack problem using a branch and bound
 * approach.
 * TODO: the solution presented below represents a "rough" attempt and is not
 * in anyway optimized.
 * TODO: solution in terms of which items are picked along the optimal path
 * could easily be recovered by associating with each node a bitvector 
 * encoding the decision made along the tree in order to arrive at that
 * particular node (e.g., for the node encoding that we did not take
 * the first item, but took the second and third items, the node's bit vector
 * would be equal to 011) 
 */
static char *
solve_knapsack_instance_bb(int n, int K, Item *items) {

        PQueue *pq;
        Node *u, *v; 
        Item tmp;
        int maxvalue;
        char *sol = malloc(128 * sizeof(char));
        
        pq = pqueue_init(n, node_get_bound);

        v = malloc(sizeof(Item));
        if (!v) allocation_error();

        v->level = -1;
        v->value = 0;
        v->weight = 0;

        pqueue_enqueue(pq, (void *) v); 

        /* While priority queue is not empty ... */
        while (pq->nElements > 1) {

                pqueue_dequeue(pq, (void **) &v, NULL);

                v->bound = bound(n, K, items, v);

                DEBUG_PRINT("maxvalue: %d\t v->bound: %f", maxvalue, v->bound);

                if (v->bound > maxvalue) {

                        /* Set u to be child that includes next item. */
                        /* Better solution: preallocate and dynamically grow
                         * array of Items to be used as nodes in search tree */
                        u = malloc(sizeof(Node));
                        if (!u) allocation_error();

                        u->level = v->level + 1;
                        u->weight = v->weight + items[u->level].weight;
                        u->value = v->value + items[u->level].value;

                        if (u->weight <= K && u->value > maxvalue) 
                                maxvalue = u->value;

                        u->bound = bound(n, K, items, u);

                        if (u->bound > maxvalue)
                                pqueue_enqueue(pq, (void *) u);
                        else free(u);

                        /* Set u to be child that does not include next item */
                        u = malloc(sizeof(Node));
                        if (!u) allocation_error();

                        u->level = v->level + 1;
                        u->weight = v->weight;
                        u->value = v->value;

                        u->bound = bound(n, K, items, u);

                        if (u->bound > maxvalue)
                                pqueue_enqueue(pq, (void *) u);
                        else free(u);

                        free(v);

                }
        }

        sprintf(sol, "%d\n", maxvalue);
        return sol;
}

/**
 * Produces relaxated optimistic estimate for value of tree below the
 * given Item.
 * Constraints are relaxed by assuming fractional parts of items can be
 * taken.
 */
static double
bound(int n, int K, Item *items, Node *x) {

        double ret;
        int j, total_weight;

        /* If weight of item exceeds capacity of knapsack, it cannot be 
         * part of any feasible solution as so its value is 0. */ 
        if (x->weight > K) return 0;


        ret = x->value;
        j = x->level + 1;
        total_weight = x->weight;

        while (j < n && (total_weight + items[j].value) <= K) {
                total_weight += items[j].weight;
                ret += items[j].value;
                j++;
        }

        /* Grab faction of jth item to fill rest of knapsack. */
        if (j < n) 
                ret += (K - total_weight) * 
                        ( (double)items[j].value / 
                          (double)items[j].weight);
        
        return ret;
}

