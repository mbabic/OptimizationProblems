/*
 * Module implementing a priority queue in the C programming language.
 * Marko Tomislav Babic - mbabic
 */

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

        PQueue *tmp;
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


PQueue *
pqueue_enqueue(Pqueue *pq, void *pqueue_elem_data) {

        PQueueElement *elem;

}
