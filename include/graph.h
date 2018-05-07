#ifndef __GRAPH__
#define __GRAPH__

#include "llist.h"

typedef struct graph *GRAPH;

GRAPH graph_create(int);

void graph_add_edge(GRAPH g, int o, int d);

LLIST graph_get_adj(GRAPH g, int n);

void graph_free(GRAPH g);

#endif
