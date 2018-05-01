#include <stdlib.h>
#include <assert.h>
#include "llist.h"

struct llist {
    struct node *head;
};

struct node {
    void *data;
    struct node *next;
};

LLIST llist_create(void) {
    LLIST new = malloc(sizeof(struct llist));
    new->head = NULL;
    return new;
}

void llist_add(LLIST l, void *d) {
    assert(l);
    struct node *new_node = malloc(sizeof(struct node));
    new_node->data = d;
    new_node->next = l->head;
    l->head = new_node;
}

void *llist_get_data(LLIST l) {
    assert(l);
    return (l->head ? l->head->data : NULL);
}

void llist_next(LLIST l) {
    assert(l);
    assert(l->head);
    l->head = l->head->next;
}

LLIST llist_clone(LLIST l) {
    assert(l);
    LLIST new = malloc(sizeof(struct llist));
    new->head = l->head;
    return new;
}

void llist_node_free(struct node *node, void (*f)(void *)) {
    if (node) {
        llist_node_free(node->next, f);
        if (f) {
            (*f)(node->data);
        }
        free(node);
    }
}

void llist_free(LLIST l, void (*f)(void *)) {
    assert(l);
    llist_node_free(l->head, f);
    free(l);
}
