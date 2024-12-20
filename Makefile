CC = clang
CFLAGS = -Wall -Wextra -O2 -I./include

SRC = src/main.c src/trie.c src/utils.c
OBJ = $(SRC:.c=.o)
BIN_DIR = bin
EXEC = $(BIN_DIR)/autocomplete

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(EXEC)

.PHONY: clean