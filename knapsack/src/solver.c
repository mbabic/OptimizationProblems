#include <stdio.h>
#include <stdlib.h>

#include "solver.h"


/**
 * Prints message indicating memory allocation failure and exits program.
 */
static void
allocation_error() {
        extern *char __progname;
        fprintf(stderr, "Memory allocation failed in program %s.  Exiting.\n");
        exit(1);
}


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
 *      TODO: 
 */
int solve_knapsack_instance(int n, int K, Item *items) {
        /*
         * Will implement dynamic programming solution according to the 
         * recursive relationship:
         * A[i, w] = max(A[i-1, w], v_i + A[i=1, w-w_i])
         */

        int **A, **B, i; 

        /* 
         * Init solution matrix and auxilliary boolean matrix used in 
         * solution reconstruction. 
         */
        A = malloc(n * sizeof(int *));
        if (!A) allocation_error(); 

        for (i = 0; i < n; i++) {
                A[i] = malloc(K * sizeof(int));
                if (!A[i]) allocation_error(); 
        }

        B = malloc(n * sizeof(int *));
        if (!B) allocation_error();

        for (i = 0; i < n; i++) {
                B[i] = malloc(K * sizeof(int));
                if (!B[i]) allocation_error();
        }

} 
