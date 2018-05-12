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

test: $(patsubst test/%.c, test/bin/%, $(wildcard test/*.c))

test/bin/%: test/%.c $(patsubst lib/%.c, obj/%.o, $(wildcard lib/*.c))
	$(CC) -o $@ $^ -Iinclude $(CFLAGS)


.PHONY: cleanall
.PHONY: clean
.PHONY: clean_obj
.PHONY: clean_test

cleanall: clean clean_test clean_obj

clean:
	rm main

clean_test:
	rm test/bin/*

clean_obj:
	rm obj/*.o

