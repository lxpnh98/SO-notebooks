SHELL=/bin/bash
PROGNAME=main
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c11 -ggdb
# disable asserts
#CFLAGS=-Wall -Wextra -pedantic -std=c11 -ggdb -DNDEBUG

$(PROGNAME): main.c $(patsubst lib/%.c, obj/%.o, $(wildcard lib/*.c))
	$(CC) -o $@ $^ -Iinclude $(CFLAGS)

obj/%.o: lib/%.c $(wildcard include/*.h)
	$(CC) -c -o $@ $< -Iinclude $(CFLAGS)

tests: $(patsubst tests/%.c, tests/bin/%, $(wildcard tests/*.c))

tests/bin/%: tests/%.c $(patsubst lib/%.c, obj/%.o, $(wildcard lib/*.c))
	$(CC) -o $@ $^ -Iinclude $(CFLAGS)


.PHONY: clean

clean:
	rm main obj/*.o

clean_tests:
	rm tests/bin/*
	rm obj/*.o
