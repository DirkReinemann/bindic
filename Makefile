CC=gcc
CFLAGS=-g3 -std=c11 -Wall -Wextra -Werror `pkg-config --libs --cflags cairo-xlib`
SOURCES=bindic.c
BIN=bindic
INSTALL_BIN=/usr/bin
FILES=$(BIN)

all: compile

compile:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BIN)

clean:
	-rm -f *.o $(BIN) *~ *.out *.log

run: compile
	./$(BIN)

install: compile
	-mkdir -p $(INSTALL_DIR)
	-@$(foreach file,$(FILES),cp $(file) $(INSTALL_DIR)/$(file);)
	-cp $(BIN).bin $(INSTALL_BIN)/$(BIN)

uninstall:
	-rm -f $(INSTALL_BIN)/$(BIN)
	-rm -rf $(INSTALL_DIR)
