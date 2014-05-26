/**
 * Simple graph ADT.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"


static void
graph_allocation_error() {
        fprintf(stderr, "Memory allocation failed. %s: %d\n", __FILE__,
                        __LINE__);
        exit(1);
}

Graph *
graph_init(int nNodes) {

        Graph *g;
        int i;

        assert(nNodes > 0);

        g = malloc(sizeof(Graph));
        if (!g) graph_allocation_error();

        g->n = nNodes;

        g->nodes = malloc(g->n * sizeof(Node));
        if (!g->nodes) graph_allocation_error(); 

        g->adj = malloc(g->n * sizeof(int *));
        if (!g->adj) graph_allocation_error();

        for (i = 0; i < g->n; i++) {

                g->nodes[i].id = i;
                g->nodes[i].color = -1;

                /* 
                 * Use calloc such that graph initially assumes no edges
                 * between nodes. 
                 */
                g->adj[i] = calloc(g->n, sizeof(int));
                if (!g->adj[i]) graph_allocation_error();
        }

        return g;
}

void
graph_add_edge(Graph *g, int u, int v) {

        int i, id, nMatches;

        assert(g != NULL);
        assert(u< g->n && v < g->n);

        g->adj[u][v] = 1;

        for (i = 0; i < g->n; i++) {

                id = g->nodes[i].id;

                if (id == u) {
                        g->nodes[i].degree++;
                        nMatches++;
                }
                if (id == v) {
                        g->nodes[i].degree++;
                        nMatches++;
                }

                if (nMatches == 2) break;
        }
}

void
graph_remove_edge(Graph *g, int u, int v) {

        int i, id, nMatches;

        assert(g != NULL);
        assert(u < g->n && v < g->n);

        g->adj[u][v] = 0;

        for (i = 0; i < g->n; i++) {

                id = g->nodes[i].id;

                if (id == u) {
                        g->nodes[i].degree++;
                        nMatches++;
                }
                if (id == v) {
                        g->nodes[i].degree++;
                        nMatches++;
                }

                if (nMatches == 2) break;
        }
}

