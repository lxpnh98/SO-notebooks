#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "graph.h"
#include "llist.h"
#include "parser.h"

#define BUF_SIZE 64
#define MAX_BUF  1024

void parse_cmd(char *buf, struct cmd *c);

void parse_file(int fd, GRAPH *execution_graph, LLIST *buf_to_write) {
    GRAPH g = graph_create(0);
    LLIST l = llist_create();
    int nbytes;
    char file_buf[BUF_SIZE];
    char *to_write;

    // output
    int output = 0;
    int begin_output_marks = 0;
    int end_output_marks = 0;

    // comando
    int cmd = 0;
    char cmd_buf[MAX_BUF];
    int k = 0; // onde escrever no buffer

    while ((nbytes = read(fd, file_buf, BUF_SIZE)) > 0) {
        to_write = malloc(sizeof(char *) * nbytes);
        int j = 0;
        for (int i = 0; i < nbytes; i++) {
            // excluir apenas o output
            if (output == 0) {
                if (file_buf[i] == '>') {
                    if (++begin_output_marks == 3) {
                        output = 1;
                        // adicionar bloco e preparar seguinte
                        j -= 2; // apagar marcas anteriores
                        struct block *b = malloc(sizeof(struct block));
                        b->size = j;
                        b->buf = to_write;
                        llist_add_tail(l, b);
                        to_write = malloc(sizeof(char *) * nbytes);
                        j = 0;
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

            // processamento de comandos
            if (cmd == 0 && file_buf[i] == '$') {
                cmd = 1;
                k = 0;
            }
            if (cmd == 1 && file_buf[i] == '\n') {
                cmd = 0;
                cmd_buf[k] = '\0';
                // cortar string principal, adicionar nodo de conteúdo NULL
                // fazer parsing do comando
                struct cmd *comando = malloc(sizeof(struct cmd));
                parse_cmd(cmd_buf, comando);
                write(1, cmd_buf, k);
                write(1, "\n", 1);
            }

            if (cmd == 1) {
                cmd_buf[k++] = file_buf[i];
            }
        }

        struct block *b = malloc(sizeof(struct block));
        b->size = j;
        b->buf = to_write;

        llist_add_tail(l, b);
    }
    *execution_graph = g;
    *buf_to_write = l;
}

void parse_cmd(char *buf, struct cmd *c) {
}

