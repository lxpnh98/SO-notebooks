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

int cmp_int(void *px, void *py) {
    assert(px); assert(py);
    int x = *(int *)px;
    int y = *(int *)py;
    if (x <  y) return -1;
    if (x == y) return  0;
    else        return  1;
}

void graph_add_edge(GRAPH g, int o, int d) {
    assert(g);
    assert(o >= 0); assert(d >= 0);
    assert(o < g->nnodes); assert(d < g->nnodes);
    int *dest = malloc(sizeof(int));
    *dest = d;
    llist_add_unique(g->nodes[o]->adj, dest, cmp_int);
}

LLIST graph_get_adj(GRAPH g, int n) {
    assert(g); assert(n < g->nnodes);
    return g->nodes[n]->adj;
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
