/*
 * Module implementating functionality with regarding to solving an instance
 * of the knapsack problem.
 * Marko Tomislav Babic - mbabic@ualberta.ca
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void
construct_solution(int *, int, int, Item *);

static char *
construct_solution_string(int *, int, int, Item *);

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
char *
solve_knapsack_instance(int n, int K, Item *items) {
        /*
         * Will implement dynamic programming solution according to the 
         * recursive relationship:
         * A[i, w] = max(A[i-1, w], v_i + A[i=1, w-w_i])
         */
        Item item;
        int *A, *ACpy, i, w; 

        /* 
         * Init solution matrix and auxilliary boolean matrix used in 
         * solution reconstruction. 
         */
        A = malloc((K + 1) * sizeof(int));
        if (!A) allocation_error(); 

        ACpy = malloc((K + 1) * sizeof(int));
        if (!ACpy) allocation_error();

        /* Set initial values. */
        for (w = 0; w <= K; w++) {
               A[w] = 0; 
        }

        /* Populate matrix of sub-solutions. */
        for (i = 0; i < n; i++) {

                for (w = 0; w <= K; w++) ACpy[w] = A[w];

                item = items[i];
                for (w = 0; w <= K; w++) {
                        if (item.weight <= w && 
                            A[w] < ACpy[w-item.weight] + item.value) {
                                A[w] = ACpy[w-item.weight] + item.value;
                        } 
                }
        }
        
        /* Construct solution from values in matrix of sub-solutions. */
        construct_solution(A, n, K, items);

        DEBUG_PRINT("Solution: %d\n", A[K]);

        return construct_solution_string(A, n, K, items);

} 

/**
 * For each item, set its isTaken variable based on the values in the 
 * sub-solution matrix.  That is, after having tabulated all entries in the
 * matrix, construct_solution uses the values to determine which items
 * were taken and which were not. 
 */
static void
construct_solution(int *A, int n, int K, Item *items) {
        int w = K, i;
        for (i = n; i > 0; i--) {
                
                if (w < items[i-1].weight) continue;

                if (A[w] == A[w-items[i-1].weight] + items[i-1].value) {
                        // Item i appeared in the solution.  Set its isTaken
                        // attribute to be true.
                        items[i-1].isTaken = 1;
                        w = w - items[i-1].weight;
                } 
                DEBUG_PRINT("Solution: item %d isTaken = %d\n",
                                i-1, items[i-1].isTaken);
        } 
}

static char *
construct_solution_string(int *A, int n, int K, Item *items) {

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

        sprintf(sol, "%d %d\n", A[K], 1);

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

