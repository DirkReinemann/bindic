CC=gcc
CFLAGS=-g3 -std=c11 -Wall -Wextra -Werror
SOURCES=bindic.c
BIN=bindic

all: compile

compile:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BIN)

clean: mods-clean
	-rm -f *.o $(BIN) *~ *.out *.log

run: compile
	./$(BIN)
