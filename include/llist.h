#ifndef __LLIST__
#define __LLIST__

typedef struct llist *LLIST;

LLIST llist_create(void);

void llist_add(LLIST l, void *d);

void llist_add_unique(LLIST l, void *d, int (*cmp)(void *, void *));

void *llist_get_data(LLIST l);

void llist_next(LLIST l);

LLIST llist_clone(LLIST l);

void llist_free(LLIST l, void (*f)(void *));

#endif
