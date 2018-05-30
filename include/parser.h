#ifndef __PARSER__
#define __PARSER__

#include "graph.h"
#include "llist.h"

struct block {
    int size;
    char *buf;
};

struct cmd {
    LLIST args;
    int input_from;
    int output_to;
};

void parse_file(int fd, GRAPH *execution_graph, LLIST *buf_to_write);

#endif
