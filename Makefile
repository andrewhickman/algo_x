#CC := g++
CC := clang++-3.8 -stdlib=libstdc++ 
SRCDIR := source
BUILDDIR := build
TARGET := bin/a.out
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := --std=c++1y -Wall -Wextra -Wshadow -pedantic -Werror -g
LIB :=
INC := -I include

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET)

tidy:
	clang-tidy-3.8

# Tests
tester:
	$(CC) $(CFLAGS) test/test.cpp $(INC) $(LIB) -o bin/test

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY:
	clean
