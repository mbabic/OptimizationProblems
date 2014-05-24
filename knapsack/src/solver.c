/*
 * Module implementating functionality with regarding to solving an instance
 * of the knapsack problem.
 * Marko Tomislav Babic - mbabic@ualberta.ca
 */

#include <stdio.h>
#include <stdlib.h>

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
 *      TODO: void for now
 */
void
solve_knapsack_instance(int n, int K, Item *items) {
        /*
         * Will implement dynamic programming solution according to the 
         * recursive relationship:
         * A[i, w] = max(A[i-1, w], v_i + A[i=1, w-w_i])
         */

        Item item;
        int **A, **B, i, w; 

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

        B = malloc((n + 1) * sizeof(int *));
        if (!B) allocation_error();

        for (i = 0; i < (n + 1); i++) {
                B[i] = malloc((K+1) * sizeof(int));
                if (!B[i]) allocation_error();
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

        /* Construct solution from value in matrix of sub-solutions. */
        w = K;
        for (i = n; i > 0; i--) {
                
                if (A[i][w] == A[i-1][w-items[i-1].weight] + items[i-1].value) {
                        // Item i appeared in the solution.  Set its isTaken
                        // attribute to be true.
                        items[i-1].isTaken = 1;
                } 
                DEBUG_PRINT("Solution: item %d isTaken = %d\n",
                                i-1, items[i-1].isTaken);
        } 

        DEBUG_PRINT("Solution: %d\n", A[n][K]);
} 
