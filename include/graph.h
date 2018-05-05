#ifndef __GRAPH__
#define __GRAPH__

typedef struct graph *GRAPH;

GRAPH graph_create(int);

void graph_add_edge(GRAPH g, int o, int d);

void graph_free(GRAPH g);

#endif
