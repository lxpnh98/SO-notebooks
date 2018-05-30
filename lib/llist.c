#include <stdlib.h>
#include <assert.h>
#include "llist.h"
#include "parser.h"

struct llist {
    struct node *head;
    struct node *tail;
};

struct node {
    void *data;
    struct node *next;
};

LLIST llist_create(void) {
    LLIST new = malloc(sizeof(struct llist));
    new->head = NULL;
    new->tail = NULL;
    return new;
}

void llist_add(LLIST l, void *d) {
    assert(l);
    struct node *new_node = malloc(sizeof(struct node));
    new_node->data = d;
    new_node->next = l->head;
    l->head = new_node;
    if (l->tail == NULL) l->tail = l->head;
}

void llist_add_tail(LLIST l, void *d) {
    assert(l);
    if (l->tail == NULL) llist_add(l, d);
    else {
        struct node *new_node = malloc(sizeof(struct node));
        new_node->data = d;
        new_node->next = NULL;
        l->tail->next = new_node;
        l->tail = new_node;
    }
}

void llist_add_unique(LLIST l, void *d, int (*cmp)(void *, void *)) {
    assert(l); assert(cmp);
    struct node *x;
    for (x = l->head; x != NULL; x = x->next) {
        if (cmp(x->data, d) == 0) break;
    }
    if (x == NULL) llist_add(l, d);
}

void *llist_get_data(LLIST l) {
    assert(l);
    return (l->head ? l->head->data : NULL);
}

void llist_next(LLIST l) {
    assert(l);
    assert(l->head);
    l->head = l->head->next;
    if (l->head == NULL) l->tail = NULL;
}

LLIST llist_clone(LLIST l) {
    assert(l);
    LLIST new = malloc(sizeof(struct llist));
    new->head = l->head;
    new->tail = l->tail;
    return new;
}

void **llist_to_array(LLIST l) {
    int len = llist_length(l);
    void **a = malloc(sizeof(void *) * len);
    struct node *h;
    int i;
    for (i = 0, h = l->head; h; i++, h = h->next) {
        a[i] = h->data;
    }
    return a;
}

void llist_insert_at(LLIST l, void *d, int index) {
    for (int i = 0; i < index; ++i) {
        llist_next(l);
    }
    struct node *new_node = malloc(sizeof(struct node));
    new_node->data = d;
    new_node->next = l->head;
    l->head = new_node;
}

int llist_length(LLIST l) {
    int length = 0;
    struct node *h;
    for (h = l->head; h; h = h->next) {
        length++;
    }
    return length;
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
