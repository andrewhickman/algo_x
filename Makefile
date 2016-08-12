#CC := g++ -fmax-errors=5
CC := clang++-3.8 -stdlib=libstdc++ -ferror-limit=5
SRCDIR := source
BUILDDIR := build
TARGET := bin/a.out

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := --std=c++1y -Wall -Wextra -Wshadow -pedantic -Werror -O3
LIB := $(OBJECTS)
INC := -I include

$(TARGET): $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning...";
	$(RM) -r $(BUILDDIR)

sudoku:
	$(CC) $(CFLAGS) tests/sudoku.cpp $(INC) $(LIB) -o bin/sudoku

sudoku_benchmark:
	$(CC) $(CFLAGS) tests/sudoku_benchmark.cpp $(INC) $(LIB) -o bin/sudoku_benchmark

# Tests
test:
	$(CC) $(CFLAGS) tests/test.cpp $(INC) $(LIB) -o bin/test

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY:
	clean
