# Program
TARGET := beditor

# Directories
INCLUDE := include
SRC := src

# Terminal Emulator
TERM := gnome-terminal

# C Compiler
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic
CINCLUDE := -I $(INCLUDE)
CLIB := -l ncurses

# Sources
CSRC := $(shell find $(SRC) -name '*.c')
COBJ := $(CSRC:.c=.o)

# Rules
%.o: %.c
	@echo "Compiling $<. . ."
	@$(CC) -c $(CINCLUDE) $(CFLAGS) -o $@ $<
	@echo "File $@ Generated."

# Targets
all: build run

build: compile link

compile: $(COBJ)

link:
	@echo "Compiling program. . ."
	@$(CC) $(CINCLUDE) -o $(TARGET) $(COBJ) $(CLIB)
	@echo "Beditor compiled."

run:
	@$(TERM) -- ./$(TARGET)
	@echo "Running Beditor application."

.PHONY: clean
clean:
	@rm $(shell find $(SRC) -name '*.o')
	@echo "Directories cleaned"
