/*
 * Module implementing a max priority queue in the C programming language.
 * Marko Tomislav Babic - mbabic
 * Some details cribbed from rosettacode.rog/wiki/Priority_queue
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
pqueue_init(int sz, double (*calculate_priority)(void *)) {

        PQueue *pq;

        pq = malloc(sizeof(PQueue));
        if (!pq) pqueue_allocation_error();
        
        sz = sz < MIN_PQUEUE_SIZE ? MIN_PQUEUE_SIZE : sz;
        pq->sz = sz;

        pq->elements = malloc(pq->sz * sizeof(PQueueElement));
        if (!pq->elements) pqueue_allocation_error();

        pq->nElements = 1;
        pq->calculate_priority = calculate_priority;

        return pq;
}


void
pqueue_enqueue(PQueue *pq, void *pqueue_elem_data) {

        double element_priority;
        int k, level;

        assert(pq->nElements <= pq->sz);

        if (pq->nElements == pq->sz) pqueue_grow(pq);

        k = pq->nElements++;
               
        element_priority = pq->calculate_priority(pqueue_elem_data);

        /* Bubble new element up the heap into correct position. */
        while ((level =  k / 2) && 
                        (element_priority > pq->elements[level].priority)) {
                
                pq->elements[k].data = pq->elements[level].data;
                pq->elements[k].priority = pq->elements[level].priority;
                k = level;
        }
        pq->elements[k].data = pqueue_elem_data;
        pq->elements[k].priority = element_priority;
}

void 
pqueue_dequeue(PQueue *pq, void **data, double *priority) {

        int k, level;
        /* If queue is empty, return null pointer. */
        if (pq->nElements <= 1 && data) *data = NULL;
        else if (pq->nElements <= 1) return;

        if (data) *data = pq->elements[1].data; 
        if (priority) *priority = pq->elements[1].priority;

        /* Re-order queue. */
        pq->nElements--;
        k = 1;
        while ((level = k * 2) < pq->nElements) {
                if (level+1 < pq->nElements &&
                    (pq->elements[level].priority < 
                     pq->elements[level+1].priority)) 
                        level++;

                if (pq->elements[pq->nElements].priority >=
                    pq->elements[level].priority) break;

                pq->elements[k].data = pq->elements[level].data;
                pq->elements[k].priority = pq->elements[level].priority;

                k = level;
        }
        pq->elements[k] = pq->elements[pq->nElements];
}


void
pqueue_free(PQueue *pq) {
        if (!pq) return;
        if (pq->elements) free(pq->elements);
}
