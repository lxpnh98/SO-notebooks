#ifndef __PARSER__
#define __PARSER__

struct block {
    int size;
    char *buf;
};

void parse_file(int fd, GRAPH *execution_graph, LLIST *buf_to_write);

#endif
