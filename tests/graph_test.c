#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "llist.h"

#define NNODES 5

int main(void) {
    srand(time(NULL));
    GRAPH g = graph_create(NNODES);
    for (int i = 0; i < 10; i++) {
        int d = rand() % NNODES;
        int o = rand() % NNODES;
        graph_add_edge(g, d, o);
        printf("%d -> %d\n", d, o);
    }
    graph_free(g);
}
