# Compiler and compiler flags
CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic

# Directories and file names
OBJECT_DIR = obj
SOURCE_DIR = src
LIBRARY_DIR = libmx
LIBRARY_NAME = $(LIBRARY_DIR)/libmx.a
LIBRARY = $(LIBRARY_NAME)
ULS = uls

# Commands
MKDIR = mkdir -p
RM = rm -rf


# Globbing patterns for source and object files
SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES = $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

# Phony targets (targets that are not actual files)
.PHONY: all uninstall clean reinstall

# Default target - builds both the library and uls
all: $(LIBRARY) $(ULS)

# Build the uls binary
$(ULS): $(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIBRARY_DIR) -lmx

# Compile source files into object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(OBJECT_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Create the object directory
$(OBJECT_DIR):
	$(MKDIR) $@

# Build the library (depends on the rules in the libmx directory)
$(LIBRARY):
	$(MAKE) -C $(LIBRARY_DIR)

# Clean up object files and the object directory
clean:
	# Remove object files
	$(RM) $(OBJECT_FILES)
	# Remove the object directory and its contents
	$(RM) $(OBJECT_DIR)

# Uninstall (clean and remove the pathfinder binary)
uninstall:
	# Call uninstall target in the libmx directory
	$(MAKE) -C $(LIBRARY_DIR) $@
	# Clean up object files and the object directory
	$(MAKE) clean
	# Remove the pathfinder binary
	$(RM) $(PATHFINDER)

# Reinstall the application
reinstall: uninstall all
