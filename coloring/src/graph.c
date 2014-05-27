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

/**
 * Initializes and returns pointer to Graph struct with given number of nodes.
 * @param int nNodes
 *      The number of nodes in the graph to be constructed.
 */
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

        g->adj = malloc(g->n * sizeof(char *));
        if (!g->adj) graph_allocation_error();

        for (i = 0; i < g->n; i++) {

                g->nodes[i].id = i;
                g->nodes[i].color = 0;
                g->nodes[i].n = g->n;
                /* 
                 * Use calloc such that graph initially assumes no edges
                 * between nodes. 
                 */
                g->adj[i] = calloc(g->n, sizeof(char));
                if (!g->adj[i]) graph_allocation_error();
        }

        return g;
}

/**
 * Returns pointer to Node struct with given id in given Graph.
 * @param Graph *g
 *      The graph in which to search for a node with the given id.
 * @param int id
 *      The id of the node to search for.
 * @return
 *      Pointer to Node struct with corresponding id in graph.
 */
Node *
graph_get_node_by_id(Graph *g, int id) {

        int i = 0;

        assert(id < g->n);

        while (i < g->n) {
                if (g->nodes[i].id == id) return &(g->nodes[i]);
                i++;
        }

        return NULL; 
}

/**
 * Add an edge between nodes with the given ids in the graph.
 * @param Graph *g
 *      The graph to which to add the edge.
 * @param int u
 *      The id of the first node in the edge.
 * @param int v
 *      The id of the second node in the edge.
 */
void
graph_add_edge(Graph *g, int u, int v) {

        Node *n;
        
        assert(g != NULL);
        assert(u< g->n && v < g->n);

        g->adj[u][v] = (char) 1;
        g->adj[v][u] = (char) 1;

        n = graph_get_node_by_id(g, u);
        n->degree++;

        n = graph_get_node_by_id(g, v);
        n->degree++;
}

/**
 * Delete an edge between nodes with the given ids in the graph.
 * @param Graph *g
 *      The graph to which to add the edge.
 * @param int u
 *      The id of the first node in the edge.
 * @param int v
 *      The id of the second node in the edge.
 */
void
graph_remove_edge(Graph *g, int u, int v) {

        Node *n;
        assert(g != NULL);
        assert(u < g->n && v < g->n);

        g->adj[u][v] = (char) 0;
        g->adj[v][u] = (char) 0;

        n = graph_get_node_by_id(g, u);
        n->degree++;

        n = graph_get_node_by_id(g, v);
        n->degree++;
}


/**
 * Updates the saturation degrees for each node in the graph.
 * TODO: clean up this implementation.
 * @param Graph *g
 *      Pointer to the graphs for which saturation degrees are to be 
 *      calculated.
 */
void
graph_update_saturation_degrees(Graph *g) {

        Node *u, *v;
        char **adj, *col;
        int i, j;
        
        assert(g != NULL);
       
        adj = g->adj;

        for (i = 0; i < g->n; i++) {

                u = &(g->nodes[i]);
              
                col = adj[i];

                u->saturation_degree = 0;

                for (j = 0; j < g->n; j++) {

                        if (col[j] == (char) 1) {
                                v = graph_get_node_by_id(g, j);

                                if (v->color && u->color != v->color)
                                        u->saturation_degree++;
                        }
                } 

        }

}

/**
 * Returns the saturation degree of the given node.  Implements the interface
 * defined by the pqueue module.
 */
double
node_calculate_priority(void *x) {
        Node *n = (Node *)x;
        return (double) ( (double) n->saturation_degree) + 
                ( (double) n->degree / (double) n->n);
}


