/**
 * Module defining functionality and attributes related to an in item
 * in the knapsack.
 */
#include <assert.h>
#include <stdlib.h>

#include "item.h"

/**
 * Calculates the priority of the given pointer to an Item struct.  Implements
 * the interface expected the PQueue ADT.
 */
double 
calculate_item_priority(void *x) {
        Item *item = (Item *) x;

        assert(item != NULL);

        if (item->weight == 0) return 0;

        return (double) ( ((double) item->value) / ((double) item->weight) );
}


