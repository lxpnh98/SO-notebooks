#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "common.h"
#include "graph.h"
#include "llist.h"
#include "parser.h"

void cria_processos(GRAPH g);

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

    cria_processos(g);

    struct block *b;
    LLIST x = llist_clone(l);
    while ((b = (struct block *)llist_get_data(x))) {
        write(1, b->buf, b->size);
        llist_next(x);
    }

    exit(0);
}

void cria_processos(GRAPH g) {
    int tam = graph_get_nnodes(g);
    pid_t pid;

    for(int i = 0; i < tam; i++) {
        /*
        LLIST adj = graph_get_adj(g, i);
        int *j;
        while ( (j = (int *)(llist_get_data(adj))) ) {
            llist_next(adj);
        }
        */
        if((pid = fork()) < 0) {
            exit(1);
        }
        if (pid == 0) {
            LLIST arg_list = ((struct cmd *)graph_get_data(g, i))->args;
            char **argv = (char **)llist_to_array(arg_list);
            execvp(argv[0], argv);
            exit(1); // em caso de erro
        } else {
            wait(NULL);
        }
    }
}
