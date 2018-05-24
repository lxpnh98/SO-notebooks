#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "llist.h"

int main(void) {
    srand(time(NULL));

    LLIST l = llist_create();
    for (int i = 0; i < 5; i++) {
        int *x = malloc(sizeof(int));
        *x = rand() % 100;
        llist_add(l, x);
    }

    LLIST x = llist_clone(l);
    int *i;
    while ((i = (int *)llist_get_data(x))) {
        printf("%d\n", *i);
        llist_next(x);
    }
    llist_free(x, NULL);

    llist_free(l, free);
    return 0;
}
