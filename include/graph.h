#ifndef __GRAPH__
#define __GRAPH__

#include "llist.h"

typedef struct graph *GRAPH;

GRAPH graph_create(int);

int graph_get_nnodes(GRAPH g);

void graph_set_nnodes(GRAPH g, int size);

void *graph_get_data(GRAPH g, int n);

void graph_set_data(GRAPH g, int n, void *d);

void graph_add_edge(GRAPH g, int o, int d);

LLIST graph_get_adj(GRAPH g, int n);

void graph_free(GRAPH g, void (*f)(void *));

#endif
