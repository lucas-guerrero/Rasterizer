#
# File       : makefile
# Licence    : see LICENCE
# Maintainer : Lucas GUERRERO
#

# Shell program used.
SHELL = /bin/sh

# Binaries directory.
BIN_DIR := bin

# Other directories.
SRC_DIR := src
OBJ_DIR := build
TEST_SRC_DIR := test

# Minwin directories .
MINWIN_DIR := minwin
MINWIN_SRC_DIR := $(MINWIN_DIR)/$(SRC_DIR)
MINWIN_HRD_DIR := $(MINWIN_DIR)/include
OBJ_DIR_MINWIN := $(MINWIN_DIR)/$(OBJ_DIR)

# File extensions.
SRC_EXT := cpp
OBJ_EXT := o
LIB_EXT := so

# Compiler and options.
CC = g++
CDEBUG = -g
INC := -Isrc/
LIBS := 
CFLAGS = -std=c++14 -fPIC -Wall -Wextra -O $(CDEBUG) $(INC)
LDFLAGS = -g


# Generate executable test files.
.PHONY: all
all: 
	cd minwin && make


.PHONY: test
test: minwin_test


minwin_test: minwin_test.o | bin
	g++ -Lminwin/bin -g -o bin/$@ build/$^ -lminwin

bin:
	mkdir -p $@

minwin_test.o: test/test_minwin.cpp | build
	g++ -c -std=c++14 -fPIC -Wall -Wextra -O $(CDEBUG) -o build/$@ $^ -I$(MINWIN_HRD_DIR) $(shell sdl2-config --cflags)

build:
	mkdir -p $@

# Cleaning.
.PHONY: clean
clean:
	$(RM) -r $(BIN_DIR)
	$(RM) -r $(OBJ_DIR)
	cd minwin && make clean

