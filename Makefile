CC=gcc
CFLAGS=-g3 -std=c11 -Wall -Wextra -Werror `pkg-config --libs --cflags gtk+-3.0` -rdynamic
SOURCES=bindic.c
BIN=bindic
INSTALL_BIN=/usr/bin
INSTALL_DIR=/usr/share/$(BIN)
FILES=$(BIN).css $(BIN).glade $(BIN)

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
