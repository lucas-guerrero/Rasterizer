#
# The Minimalist Windowing Library
#
# File      : makefile
# Copyright : (c) 2021 Université d'Artois
# Author    : Tiago de Lima <tiago.delima@univ-artois.fr>
#

# TODO: LICENCE HERE!

#------------------------------------------------------------------------------
# Configuration
#------------------------------------------------------------------------------

SHELL := /bin/sh

# Set OS
OS := $(shell uname)

# Set compiler
CXX := g++

std := -std=c++14
cxxdebug := -g -U NDEBUG 
warns := -Wall -Wextra
pic := -fPIC
sdl2_inc := -Iinclude $(shell sdl2-config --cflags)
sdl2_ldflags := #-L/usr/lib/x86_64-linux-gnu/
sdl2_ldlibs  := $(shell sdl2-config --libs) -lSDL2_ttf
ldshared := -shared
lddebug := -g

# Set preprocessor flags
CPPFLAGS :=
# Set c++ compiler flags
CXXFLAGS := $(std) $(pic) $(cxxdebug) $(warns) $(sdl2_inc)
# Set linker flags
LDFLAGS := $(lddebug) $(sdl2_ldflags)
# Set libraries used in linking
LDLIBS := $(sdl2_ldlibs)

# Set extensions
cppext := cpp
hdrext := h
objext := o
libext := so

# Set directories
bindir := bin
objdir := build
main_srcdir := src
main_incdir := include
test_srcdir := test

# Set main target
main_target := $(bindir)/libminwin.$(libext)
main_srcs   := $(wildcard $(main_srcdir)/*.$(cppext))
main_hdrs   := $(wildcard $(main_srcdir)/*.$(hdrext)) $(wildcard $(main_hdrdir)/*.$(hdrext))
main_objs   := $(patsubst $(main_srcdir)/%,$(objdir)/%,$(main_srcs:.$(cppext)=.$(objext)))

test_target := $(bindir)/test_minwin
test_srcs   := $(wildcard $(test_srcdir)/*.$(cppext))
test_objs   := $(patsubst $(test_srcdir)/%,$(objdir)/%,$(test_srcs:.$(cppext)=.$(objext)))

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------

#
# Main target
#

# Generate main target file
$(main_target) : $(main_objs) | $(bindir)
	$(CXX) $(ldshared) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Generate bindir if necessary
$(main_target) : | $(bindir)

.PHONY: all
all: $(main_target)

#
# Other binaries
#

# Generate tester
$(test_target) : $(test_objs) | $(bindir)
	$(CXX) $(LDFLAGS) -L$(bindir) -o $@ $^ -lminwin

# Generate bindir if necessary
$(test_target) : | $(bindir)

#
# Objects
#

# Generate main object files
$(main_objs) : $(objdir)/%.$(objext) : $(main_srcdir)/%.$(cppext) $(main_hdrs)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# Generate objdir if necessary
$(main_objs) : | $(objdir)

# Generate test object files
# IMPORTANT NOTE: This rule does not check whether other headers included by the
#                 source file .cpp have been changed.
$(test_objs) : $(objdir)/%.$(objext) : $(test_srcdir)/%.$(cppext)\
                                       $(test_srcdir)/%.$(hdrext)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

# Generate objdir if necessary
$(test_objs) : | $(objdir)

# Other

$(bindir) :
	mkdir -p $(bindir)

$(objdir) :
	mkdir -p $(objdir)

# Cleaning.
.PHONY: clean
clean:
	$(RM) $(main_target)
	$(RM) $(test_target)
	$(RM) $(main_objs)
	$(RM) $(test_objs)

# Test
.PHONY: test
test: tester
	$(test_target)

# TODO
#.PHONY: doc
#doc:

# TODO
#.PHONY: dist
#dist:

