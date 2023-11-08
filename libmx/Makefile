# Makefile for a C library

# Output library name
LIBRARY_NAME = libmx.a

# Source and object directories
SOURCE_DIR = src
OBJECT_DIR = obj
INCLUDE_DIR = inc

# Source files and object files
SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES = $(patsubst $(SOURCE_DIR)/%.c,$(OBJECT_DIR)/%.o,$(SOURCE_FILES))

# Header files
INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.h)

# Compiler and flags
CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -g

# Archive tool and flags
AR = ar
AFLAGS = rcs

# Commands
MKDIR = mkdir -p
RM = rm -rf

# Phony targets
.PHONY: all uninstall clean reinstall

# Default target
all: $(LIBRARY_NAME)

# Rule to build the library
$(LIBRARY_NAME): $(OBJECT_FILES)
	$(AR) $(AFLAGS) $@ $^

# Rule to build object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(INCLUDE_FILES) | $(OBJECT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

# Ensure object directory exists
$(OBJECT_DIR):
	$(MKDIR) $@

# Clean up object files
clean:
	$(RM) $(OBJECT_DIR)

# Remove the library and object files
uninstall: clean
	$(RM) $(LIBRARY_NAME)

# Uninstall and then build the library
reinstall: uninstall all
