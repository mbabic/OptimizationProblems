/*
 * Implementation of generic priority queue in the C language.
 * Marko Tomislav Babic - mbabic@ualberta.ca
 */
#ifndef PQUEUE_H
#define PQUEUE_H

/**
 * Constant defining minimum number of elements to allocate for a new 
 * pqueue.
 */
#define MIN_PQUEUE_SIZE 1024

typedef struct {
        /** Pointer to data associated with queue element. */
        void *data;  

        /** The priority of the element. */   
        int priority;
} PQueueElement;


typedef struct {
        /** Array of elements in queue. */
        PQueueElement *elements;

        /**
         * Pointer to function used to calculate the integer priority of
         * elements in the queue.
         */
        void *calculate_priority;
        
        /** 
         * The max number of elements that can be stored in queue (will
         * grow dynamically.)
         */
        int sz;

        /**
         * The number of elements currently in the queue.
         */
        int nElements;
} PQueue;



PQueue *pqueue_init(int, void *);

PQueue *pqueue_enqueue(PQueue *, void *);

PQueue *pqueue_dequeue(PQueue *);
#endif