CC=clang++
CFLAGS=-Wall -Wextra -O2 -std=c++23 -I./include

SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(SRCS:.cpp=.o)

BINDIR=bin
BIN=$(BINDIR)/autocomplete


$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)

.PHONY: clean