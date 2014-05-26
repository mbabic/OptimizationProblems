/**
 * Module defining functionality with regard to a node in the solution
 * tree.
 */
#ifndef NODE_H
#define NODE_H

typedef struct {
        double bound;
        int value;
        int weight;
        int level;
} Node;

double 
node_get_bound(void *);

#endif
