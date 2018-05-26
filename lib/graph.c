#include <stdlib.h>
#include <assert.h>
#include "llist.h"
#include "graph.h"

struct graph {
    int nnodes;
    struct node **nodes;
};

struct node {
    void *data;
    LLIST adj;
};

GRAPH graph_create(int nnodes) {
    assert(nnodes >= 0);
    GRAPH new = malloc(sizeof(struct graph));
    new->nodes = (nnodes > 0 ? malloc(sizeof(struct node *) * nnodes) : NULL);
    new->nnodes = nnodes;
    for (int i = 0; i < nnodes; i++) {
        new->nodes[i] = malloc(sizeof(struct node));
        new->nodes[i]->data = NULL;
        new->nodes[i]->adj = llist_create();
    }
    return new;
}

int graph_get_nnodes(GRAPH g) {
    assert(g);
    return g->nnodes;
}


void graph_set_nnodes(GRAPH g, int size) {
    assert(g);
    if (size > g->nnodes) {
        g->nodes = realloc(g->nodes, sizeof(struct node *) * size);
        for (int i = g->nnodes; i < size; i++) {
            g->nodes[i] = malloc(sizeof(struct node));
            g->nodes[i]->data = NULL;
            g->nodes[i]->adj = llist_create();
        }
        g->nnodes = size;
    }
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

void *graph_get_data(GRAPH g, int n) {
    assert(g); assert(n < g->nnodes);
    return g->nodes[n]->data;
}

void graph_set_data(GRAPH g, int n, void *d) {
    assert(g); assert(n < g->nnodes);
    g->nodes[n]->data = d;
}

LLIST graph_get_adj(GRAPH g, int n) {
    assert(g); assert(n < g->nnodes);
    return g->nodes[n]->adj;
}

void graph_node_free(struct node *n, void (*f)(void *)) {
    if (f) {
        (*f)(n->data);
    }
    llist_free(n->adj, free);
    free(n);
}

void graph_free(GRAPH g, void (*f)(void *)) {
    assert(g);
    for (int i = 0; i < g->nnodes; i++) {
        graph_node_free(g->nodes[i], f);
    }
    free(g);
}
