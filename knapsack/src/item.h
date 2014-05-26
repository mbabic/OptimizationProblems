/*
 * Module defining functionality and attributes related to an item in the
 * knapsack.
 */
#ifndef ITEM_H
#define ITEM_H

typedef struct {
        int isTaken;    /* Boolean flag indicating whether item is put in
                         * knapsack or not. */
        int value;      /* The item's value. */
        int weight;     /* The items' weight. */
        int id;         /* The item's id identified the position in which it
                         * was defined in the original input file. */
} Item;

double calculate_item_priority(void *);

#endif
