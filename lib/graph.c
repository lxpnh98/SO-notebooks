#include <stdlib.h>
#include <assert.h>
#include "graph.h"

struct graph {
    int nnodes;
    struct node *nodes;
};

struct node {
    struct adj_list *adj;
};

struct adj_list {
    int dest;
    struct adj_list *next;
};

GRAPH graph_create(int nnodes) {
    assert(nnodes >= 0);
    GRAPH new = malloc(sizeof(struct graph));
    new->nodes = malloc(sizeof(struct node) * nnodes);
    new->nnodes = nnodes;
    for (int i = 0; i < nnodes; i++) {
        new->nodes[i].adj = NULL;
    }
    return new;
}

void graph_add_edge(GRAPH g, int o, int d) {
    assert(g);
    assert(o >= 0); assert(d >= 0);
    assert(o < g->nnodes);
    struct adj_list *new_edge = malloc(sizeof(struct adj_list));
    new_edge->dest = d;
    new_edge->next = g->nodes[o].adj;
    g->nodes[o].adj = new_edge;
}

