#include <stdlib.h>
#include <assert.h>
#include "llist.h"
#include "graph.h"

struct graph {
    int nnodes;
    struct node **nodes;
};

struct node {
    LLIST adj;
};

GRAPH graph_create(int nnodes) {
    assert(nnodes >= 0);
    GRAPH new = malloc(sizeof(struct graph));
    new->nodes = malloc(sizeof(struct node *) * nnodes);
    new->nnodes = nnodes;
    for (int i = 0; i < nnodes; i++) {
        new->nodes[i] = malloc(sizeof(struct node));
        new->nodes[i]->adj = llist_create();
    }
    return new;
}

void graph_add_edge(GRAPH g, int o, int d) {
    assert(g);
    assert(o >= 0); assert(d >= 0);
    assert(o < g->nnodes); assert(d < g->nnodes);
    int *dest = malloc(sizeof(int));
    *dest = d;
    llist_add(g->nodes[o]->adj, dest);
}

void graph_node_free(struct node *n) {
    llist_free(n->adj, free);
    free(n);
}

void graph_free(GRAPH g) {
    assert(g);
    for (int i = 0; i < g->nnodes; i++) {
        graph_node_free(g->nodes[i]);
    }
    free(g);
}
