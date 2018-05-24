#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>

#include "common.h"
#include "graph.h"
#include "llist.h"
#include "parser.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        char e[] = "Missing argument.";
        write(2, argv[0], strlen(argv[0]));
        write(2, ": ", 2);
        write(2, e, strlen(e));
        write(2, "\n", 1);
        exit(1);
    }
    LLIST l;
    GRAPH g;
    int fd = open(argv[1], O_RDONLY);

    parse_file(fd, &g, &l);

    struct block *b;
    LLIST x = llist_clone(l);
    while ((b = (struct block *)llist_get_data(x))) {
        write(1, b->buf, b->size);
        llist_next(x);
    }

    exit(0);
}
