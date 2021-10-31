#
# File       : makefile
# Licence    : see LICENCE
# Maintainer : <your name here>
#

# Shell program used.
SHELL = /bin/sh

# Binaries directory.
BIN_DIR := bin

# Other directories.
SRC_DIR := src
OBJ_DIR := build
TEST_SRC_DIR := test

# File extensions.
SRC_EXT := cpp
OBJ_EXT := o

# Compiler and options.
CC = g++
CDEBUG = -g
OS := $(shell uname)
ifeq ($(OS), Linux)
    INC := -Isrc/
    LIBS := 
else ifeq ($(OS), Darwin)
    INC := -Isrc/
    LIBS := 
endif
CFLAGS = -std=c++11 -Wall -O $(CDEBUG) $(INC)
LDFLAGS = -g

# Find all source file names.
SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
# Generate object file names from source file names.
OBJ_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(OBJ_DIR)/%.$(OBJ_EXT), $(SRC_FILES))
# Find all test source file names.
TEST_SRC_FILES := $(wildcard $(TEST_SRC_DIR)/*.$(SRC_EXT))
# Generate test object file names from test source file names.
TEST_OBJ_FILES := $(patsubst $(TEST_SRC_DIR)/%.$(SRC_EXT), $(OBJ_DIR)/%.$(OBJ_EXT), $(TEST_SRC_FILES))
# Generate test binary file names from test source file names.
TEST_BIN_FILES := $(patsubst $(TEST_SRC_DIR)/%.$(SRC_EXT), $(BIN_DIR)/%, $(TEST_SRC_FILES))

$(OBJ_FILES): $(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

# Generate executable test files.
.PHONY: all
all: $(TEST_BIN_FILES)

# Create test_vector
$(BIN_DIR)/test_vector: $(OBJ_DIR)/test_vector.o
	mkdir -p $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

# Create test_matrix
$(BIN_DIR)/test_matrix: $(OBJ_DIR)/test_matrix.o
	mkdir -p $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_OBJ_FILES): $(OBJ_DIR)/%.$(OBJ_EXT): $(TEST_SRC_DIR)/%.$(SRC_EXT) 
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

# Cleaning.
.PHONY: clean
clean:
	$(RM) $(BIN_FILE)
	$(RM) $(OBJ_FILES)
	$(RM) $(TEST_BIN_FILES)
	$(RM) $(TEST_OBJ_FILES)

