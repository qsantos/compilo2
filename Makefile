CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pedantic -ansi -std=c99 -D_XOPEN_SOURCE=700 -O3
LDFLAGS = -O3
TARGETS = compilo

all: $(TARGETS)

compilo: main.o util.o ast.o print.o lexer.o parser.o htable.o symbol.o ir.o ast2ir.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

lexer.c: lexer.l parser.h
	flex -o $@ $<

%.c %.h: %.y
	bison --defines=$*.h -o $*.c $<

clean:
	rm -f *.o lexer.c parser.h parser.c

destroy: clean
	rm -f $(TARGETS)

rebuild: destroy all
