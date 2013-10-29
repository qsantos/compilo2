CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pedantic -ansi -std=c99 -D_XOPEN_SOURCE=700 -O3
LDFLAGS = -O3
TARGETS = compilo

all: $(TARGETS)

compilo: main.o util.o ast.o print.o lexer.o parser.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.c: %.l
	flex -o $@ $<

%.c: %.y
	bison -o $@ $<

clean:
	rm -f *.o lexer.c parser.c

destroy: clean
	rm -f $(TARGETS)

rebuild: destroy all
