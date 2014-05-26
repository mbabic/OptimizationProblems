/*
 * Module implementing a priority queue in the C programming language.
 * Marko Tomislav Babic - mbabic
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pqueue.h"

/**
 * Prints memory allocation error message.
 */
static void
pqueue_allocation_error() {
        fprintf(stderr, "Memory allocation failed.\n %s: %d\n", __FILE__,
                        __LINE__);
        exit(1);
}

/**
 * Allocates more memory to accomodate growing pqueue.
 * @param PQueue *pq
 *      Pointer to the pqueue to be grown.
 */
static void
pqueue_grow(PQueue *pq) {

        PQueueElement *tmp;
        pq->sz *= 2;

        tmp = realloc(pq->elements, pq->sz * sizeof(PQueueElement));
        if (!tmp) pqueue_allocation_error();

        pq->elements = tmp;

}

PQueue *
pqueue_init(int sz, void *calculate_priority) {

        PQueue *pq;

        pq = malloc(sizeof(PQueue));
        if (!pq) pqueue_allocation_error();
        
        sz = sz < MIN_PQUEUE_SIZE ? MIN_PQUEUE_SIZE : sz;
        pq->sz = sz;

        pq->elements = malloc(pq->sz * sizeof(PQueueElement));
        if (!pq->elements) pqueue_allocation_error();

        pq->nElements = 0;
        pq->calculate_priority = calculate_priority;

        return pq;
}


void
pqueue_enqueue(PQueue *pq, int *pqueue_elem_data) {

        int k, level, element_priority;

        assert(pq->nElements <= pq->sz);

        if (pq->nElements == pq->sz) pqueue_grow(pq);

        k = pq->sz;
        pq->sz++; 
               
        element_priority = pq->calculate_priority(pqueue_elem_data);

        /* Bubble new element up the heap until */
        while (1) {
                level = k / 2;
                if (element_priority < pq->elements[k].priority) { 
                        pq->elements[k] = pq->elements[level];
                        k = level;
                } else {
                        break;
                }
        }

        pq->elements[k].data = pqueue_elem_data;
        pq->elements[k].priority = element_priority;
}
