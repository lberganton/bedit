# Program
TARGET := bedit

# Directories
INCLUDE := include
SRC := src
TEST := test

# Test Config
TESTFILE := rocky.txt
TERM := gnome-terminal

# C Compiler
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic
CINCLUDE := -I $(INCLUDE)
CLIB := -l ncursesw

# Sources
CSRC := $(shell find $(SRC) -name '*.c')
COBJ := $(CSRC:.c=.o)

# Rules
%.o: %.c
	@echo "Compiling $<. . ."
	@$(CC) -c $(CINCLUDE) $(CFLAGS) -o $@ $<
	@echo "File $@ Generated."

# Targets
all: build test

build: compile link

compile: $(COBJ)

link:
	@echo "Compiling program. . ."
	@$(CC) $(CINCLUDE) -o $(TARGET) $(COBJ) $(CLIB)
	@echo "Beditor compiled."

.PHONY: test
test:
	@$(TERM) -- ./$(TARGET) $(TEST)/$(TESTFILE)
	@echo "Running Beditor application with $(TESTFILE) file."

.PHONY: clean
clean:
	@rm $(shell find $(SRC) -name '*.o')
	@echo "Directories cleaned"
