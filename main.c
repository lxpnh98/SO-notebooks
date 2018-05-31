#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "common.h"
#include "graph.h"
#include "llist.h"
#include "parser.h"

void make_input_fifos(GRAPH g);
void make_output_fifos(GRAPH g);
void execute_graph(GRAPH g);
void write_to_file(LLIST l, int fd);

void write_outputs(GRAPH g, LLIST l) {
    int tam = graph_get_nnodes(g);
    char path[10];
    int fd;
    int nbytes;
    char buf[1024];
    char buf_maior[1032];
    int index;
    int first;
    struct block *b;

    for(int i = 0; i < tam; i++) {
        first = 1;
        fprintf(stderr, "Output %d:\n", i);
        sprintf(path, "o%d", i);
        fd = open(path, O_RDONLY);
        struct cmd *cmd = ((struct cmd *)graph_get_data(g, i));
        index = cmd->output_to;
        while ((nbytes = read(fd, buf, 1023)) > 0) {
            write(2, buf, nbytes);
            if (first) {
                sprintf(buf_maior,">>>\n%s",buf);
            }
            b = malloc(sizeof(struct block));
            if (first) {
                b->buf = mystrdup(buf_maior);
                b->size = nbytes + 4;
            } else {
                b->buf = mystrdup(buf);
                b->size = nbytes;
            }
            llist_insert_at(l, b, index);
            index++;
            first = 0;
        }
        b = (struct block *)llist_get_data_at(l, index);
        strcpy(buf_maior, b->buf);
        memcpy(buf_maior + b->size,"<<<", 3);
        b->buf = mystrdup(buf_maior);
        b->size += 3;
        close(fd);
    }
}

int main(int argc, char *argv[]) {
    char output_path[64];
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
    make_input_fifos(g);
    make_output_fifos(g);
    execute_graph(g);
    write_outputs(g, l);
    sprintf(output_path, "%s_o", argv[1]);
    int output_fd = open(output_path, O_CREAT | O_WRONLY, 0644);
    write_to_file(l, output_fd);

    exit(0);
}

void make_input_fifos(GRAPH g) {
    int tam = graph_get_nnodes(g);
    char path[10];
    for(int i = 0; i < tam; i++) {
        if (((struct cmd *)graph_get_data(g, i))->input_from > 0) {
            sprintf(path, "i%d", i);
            fprintf(stderr, "Criou fifo %s\n", path);
            mkfifo(path, 0600);
        }
    }
}

void make_output_fifos(GRAPH g) {
    int tam = graph_get_nnodes(g);
    char path[10];
    for(int i = 0; i < tam; i++) {
        sprintf(path, "o%d", i);
        fprintf(stderr, "Criou fifo %s\n", path);
        mkfifo(path, 0600);
    }
}

void execute_distribuidor(GRAPH g, int i, int pipe[]);

void execute_graph(GRAPH g) {
    int tam = graph_get_nnodes(g);
    pid_t pid;
    char path[10];
    int input_fd;
    int output_pipe[2];

    for(int i = 0; i < tam; i++) {

        // redirecionar output e distribuir
        pipe(output_pipe);
        execute_distribuidor(g, i, output_pipe);

        // executar comando
        if((pid = fork()) == 0) {
            // abrir e redirecionar fifo de input
            if (((struct cmd *)graph_get_data(g, i))->input_from > 0) {
                sprintf(path, "i%d", i);
                input_fd = open(path, O_RDONLY); // (*)
                fprintf(stderr, "Abriu fifo '%s'.\n", path);
                dup2(input_fd, 0);
            }

            close(output_pipe[0]);
            dup2(output_pipe[1], 1);
            LLIST arg_list = ((struct cmd *)graph_get_data(g, i))->args;
            char **argv = (char **)llist_to_array(arg_list);
            fprintf(stderr, "A executar comando '%s'.\n", argv[0]);
            execvp(argv[0], argv);
            exit(1); // em caso de erro
        } else {
            if (((struct cmd *)graph_get_data(g, i))->input_from > 0) {
                wait(NULL); // se o programa que dá o input acabar primeiro que o que recebe, não funciona (hangs ao abrir o fifo para escrever (*) )
            }
            close(output_pipe[1]);
        }
    }
}

void execute_distribuidor(GRAPH g, int i, int pipe[]) {
    char path[10];
    LLIST arg_list = llist_create();

    // nome do programa
    llist_add_tail(arg_list, "./distribuidor");

    // ficheiro de output para escrever no ficheiro resultado
    sprintf(path, "o%d", i);
    llist_add_tail(arg_list, mystrdup(path));

    // adicionar fifos em que escrever
    LLIST adj = llist_clone(graph_get_adj(g, i));
    int *j;
    while ( (j = (int *)(llist_get_data(adj))) ) {
        sprintf(path, "i%d", *j);
        fprintf(stderr, "Argument: %s\n", path);
        llist_add_tail(arg_list, mystrdup(path));
        llist_next(adj);
    }
    llist_add_tail(arg_list, NULL);

    // executar programa
    char **argv = (char **)llist_to_array(arg_list);
    if (fork() == 0) {
        close(pipe[1]);
        dup2(pipe[0], 0);
        fprintf(stderr, "A executar distribuidor %d.\n", i);
        execvp(argv[0], argv);
        fprintf(stderr, "Erro ao executar distribuidor.\n");
        exit(1); // em caso de erro
    }
}

void write_to_file(LLIST l, int fd) {
    struct block *b;
    LLIST x = llist_clone(l);
    while ((b = (struct block *)llist_get_data(x))) {
        write(fd, b->buf, b->size);
        llist_next(x);
    }
}

