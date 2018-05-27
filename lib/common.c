#include <stdlib.h>
#include <string.h>

char *mystrdup(char *s) {
    int len = strlen(s);
    char *r = malloc(sizeof(char *) * len);
    memcpy(r, s, len);
    return r;
}

