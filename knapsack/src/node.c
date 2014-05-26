#include "node.h"

/**
 * Returns nodes bounds.  Implements the interface expected by the pqueue adt.
 */
double
node_get_bound(void *x) {
        Node *n = (Node *) x;
        return n->bound;
}
