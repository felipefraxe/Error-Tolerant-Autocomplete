CC = clang
CFLAGS = -Wall -Wextra -O2 -I./include

SRC = src/main.c src/trie.c
OBJ = $(SRC:.c=.o)
EXEC = autocomplete.exe

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(EXEC)

.PHONY: clean