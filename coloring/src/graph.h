/**
 * Simple undirected graph ADT.
 * Marko Tomislav Babic - mbabic@ualberta.ca
 */
#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
        /** 
         * Id of graph (i.e., its index in Graph.nodes and
         * Graph.adjacency matrix.
         */
        int id;

        /** The color given to the node. */
        int color;       
} Node;

typedef struct {
        Node u;
        Node v;
} Edge;

typedef struct {
        
        /** Number of nodes in the graph. */
        int n;

        /** Array of nodes in the graph. */
        Node *nodes;

        /** Adjacency matrix encoding edges. */
        int **adj;


} Graph;

Graph * graph_init(int);

void graph_add_edge(Graph *, int, int);

void graph_remove_edge(Graph *, int, int);

#endif
