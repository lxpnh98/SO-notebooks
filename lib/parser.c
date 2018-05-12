#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graph.h"
#include "llist.h"
#include "parser.h"

#define BUF_SIZE 64

void parse_file(int fd, GRAPH *execution_graph, LLIST *buf_to_write) {
    GRAPH g = graph_create(0);
    LLIST l = llist_create();
    int nbytes;
    char file_buf[BUF_SIZE];
    char *to_write;

    int output = 0;
    int begin_output_marks = 0;
    int end_output_marks = 0;

    while ((nbytes = read(fd, file_buf, BUF_SIZE)) > 0) {
        to_write = malloc(sizeof(char *) * nbytes);
        int j = 0;
        for (int i = 0; i < nbytes; i++) {
            if (output == 0) {
                if (file_buf[i] == '>') {
                    if (++begin_output_marks == 3) {
                        output = 1;
                        j -= 2; // apagar marcas anteriores
                    }
                } else {
                    begin_output_marks = 0;
                }
                if (output == 0) {
                    to_write[j++] = file_buf[i];
                }
            } else { // output == 1
                if (file_buf[i] == '<') {
                    if (++end_output_marks == 3) {
                        output = 0;
                    }
                } else {
                    end_output_marks = 0;
                }
            }
        }

        //memcpy(to_write, file_buf, nbytes);

        struct block *b = malloc(sizeof(struct block));
        b->size = j;
        b->buf = to_write;

        llist_add_tail(l, b);
    }
    *execution_graph = g;
    *buf_to_write = l;
}
