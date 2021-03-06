#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "graph.h"
#include "llist.h"
#include "parser.h"

#define BUF_SIZE 64
#define MAX_BUF  1024

void parse_cmd(char *buf, struct cmd *c);

void add_cmd_to_graph(GRAPH g, struct cmd *comando);

void parse_file(int fd, GRAPH *execution_graph, LLIST *buf_to_write) {
    GRAPH g = graph_create(0);
    LLIST l = llist_create();
    int nbytes;
    char file_buf[BUF_SIZE];
    char *to_write;
    int nblocks = -1;

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
                        nblocks++;
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
                // adicionar bloco e preparar seguinte
                nblocks++;
                struct block *b = malloc(sizeof(struct block));
                b->size = j;
                b->buf = to_write;
                llist_add_tail(l, b);
                to_write = malloc(sizeof(char *) * nbytes);
                j = 0;
                // fazer parsing do comando
                struct cmd *comando = malloc(sizeof(struct cmd));
                parse_cmd(cmd_buf, comando);
                add_cmd_to_graph(g, comando);
                comando->output_to = nblocks;
                nblocks++;
            }

            if (cmd == 1) {
                cmd_buf[k++] = file_buf[i];
            }
        }

        nblocks++;
        struct block *b = malloc(sizeof(struct block));
        b->size = j;
        b->buf = to_write;
        llist_add_tail(l, b);
    }
    *execution_graph = g;
    *buf_to_write = l;
}

void parse_cmd(char *buf, struct cmd *c) {
    buf++; // ignorar '$' inicial
    // detetar se tem redirecionamento de input
    char *pipe = strchr(buf, '|');
    int input_from = 0;
    if (pipe) {
        // determinar qual o input
        char *tmp = buf;
        buf = pipe + 1;
        *pipe = '\0';
        if (sscanf(tmp, "%d", &input_from) != 1) {
            input_from = 1;
        }
        c->input_from = input_from;
    }
    // construir lista de argumentos
    LLIST tokens = llist_create();
    char *token = strtok(buf, " ");
    while (token) {
        llist_add_tail(tokens, mystrdup(token));
        token = strtok(NULL, " ");
    }
    llist_add_tail(tokens, NULL); // terminar a lista de argumentos
    c->args = tokens;
}

void add_cmd_to_graph(GRAPH g, struct cmd *comando) {
    int new_node = graph_get_nnodes(g);
    graph_set_nnodes(g, new_node + 1);
    if (comando->input_from != 0) {
        graph_add_edge(g, new_node - comando->input_from, new_node);
    }
    graph_set_data(g, new_node, comando);
}
