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

        /**
         * The degree over the node (i.e., the number of nodes in the graph
         * with which it shares and edge.
         */
        int degree;

        /**
         * The number of colors to which the node is adjacent.
         */
        int saturation_degree;

        /** The color given to the node. */
        int color;       

        /**
         * Number of nodes in the graph in which the Node appears.  Convenient
         * in calculations of Node's key in priority queue.
         */
        int n;
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
        char **adj;


} Graph;

Graph * graph_init(int);

void graph_add_edge(Graph *, int, int);

void graph_remove_edge(Graph *, int, int);

void graph_get_adjacent_nodes(Graph *, int);

void graph_color_node(Graph *, int, int);

int graph_get_lowest_available_color(Graph *, Node *);

Node * graph_get_node_by_id(Graph *, int);

void graph_update_saturation_degrees(Graph *);

int graph_is_valid_coloring(Graph *, int);

double node_calculate_priority(void *);

#endif
