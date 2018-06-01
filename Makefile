SHELL=/bin/bash
PROGNAME=notebook
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -ggdb
# disable asserts
#CFLAGS=-Wall -Wextra -pedantic -std=c11 -ggdb -DNDEBUG

all: distribuidor $(PROGNAME)

$(PROGNAME): main.c $(patsubst lib/%.c, obj/%.o, $(wildcard lib/*.c))
	$(CC) -o $@ $^ -Iinclude $(CFLAGS)

obj/%.o: lib/%.c $(wildcard include/*.h)
	$(CC) -c -o $@ $< -Iinclude $(CFLAGS)

test: $(patsubst test/%.c, test/bin/%, $(wildcard test/*.c))

test/bin/%: test/%.c $(patsubst lib/%.c, obj/%.o, $(wildcard lib/*.c))
	$(CC) -o $@ $^ -Iinclude $(CFLAGS)


.PHONY: clean

clean:
	rm $(PROGNAME) distribuidor i* o* test/bin/* obj/*.o 2> /dev/null || true # ignorar erros
