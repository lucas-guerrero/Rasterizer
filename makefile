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
HRD_DIR := include

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
INC := -I$(HRD_DIR) -I$(MINWIN_HRD_DIR) $(shell sdl2-config --cflags)
LIBS := 
CFLAGS = -std=c++14 -fPIC -Wall -Wextra -O $(CDEBUG) $(INC)
LDFLAGS = -g


# find all file cpp
FILE_SRC := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
FILE_OBJ   := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(FILE_SRC:.$(SRC_EXT)=.$(OBJ_EXT)))


# Generate executable test files.
.PHONY: all
all: Minwin $(FILE_OBJ) bin/rasterise

Minwin:
	cd minwin && make

bin/rasterise: build/rasterise.o $(FILE_OBJ) | bin
	g++ -Lminwin/bin -g -o $@ $^ -lminwin

build/rasterise.o: test/rasterise.cpp | build
	g++ -c -std=c++14 -fPIC -Wall -Wextra -O $(CDEBUG) -o $@ $^ -I$(HRD_DIR) -I$(MINWIN_HRD_DIR) $(shell sdl2-config --cflags)

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(FILE_OBJ): $(OBJ_DIR)/%.$(OBJ_EXT) : $(SRC_DIR)/%.$(SRC_EXT)
	$(CC) -c $(CFLAGS) -o $@ $<

$(FILE_OBJ): | $(OBJ_DIR)

# Cleaning.
.PHONY: clean
clean:
	$(RM) -r $(BIN_DIR)
	$(RM) -r $(OBJ_DIR)
	cd minwin && make clean

