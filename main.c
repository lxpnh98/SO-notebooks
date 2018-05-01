#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "llist.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        char e[] = "Missing argument.";
        write(2, argv[0], strlen(argv[0]));
        write(2, ": ", 2);
        write(2, e, strlen(e));
        write(2, "\n", 1);
        exit(1);
    }
    LLIST l = llist_create();
    assert(0);
    exit(0);
}
