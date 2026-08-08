# Guess The Number - build
#
# C11, no dependencies. `make help` lists the targets.

CC        ?= cc
CSTD       = -std=c11
WARNINGS   = -Wall -Wextra -Wpedantic -Wshadow -Wconversion
OPTIMIZE  ?= -O2
CFLAGS    ?= $(CSTD) $(WARNINGS) $(OPTIMIZE) -IInc

SRCDIR     = Src
INCDIR     = Inc
TESTDIR    = Tests
OBJDIR     = Obj
BUILDDIR   = Build

TARGET      = $(BUILDDIR)/guessthenumber
TEST_TARGET = $(BUILDDIR)/guessthenumber_tests

# Game.c is shared: the game links it, and so do the tests. main.c is the
# only file the tests cannot link, because it has its own main().
CORE_SOURCES = $(SRCDIR)/Game.c
CORE_OBJECTS = $(CORE_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

HEADERS = $(wildcard $(INCDIR)/*.h)

.PHONY: all run test debug clean distclean help

all: $(TARGET)

$(TARGET): $(CORE_OBJECTS) $(OBJDIR)/main.o | $(BUILDDIR)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Built $@"

$(TEST_TARGET): $(CORE_OBJECTS) $(OBJDIR)/test_game.o | $(BUILDDIR)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Built $@"

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR) $(BUILDDIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

debug:
	$(MAKE) OPTIMIZE="-O0 -g" all test

clean:
	rm -rf $(OBJDIR) $(BUILDDIR)

distclean: clean
	rm -rf build

help:
	@echo "Targets:"
	@echo "  all        Build the game (default)"
	@echo "  run        Build and play"
	@echo "  test       Build and run the test suite"
	@echo "  debug      Unoptimised build with symbols, then test"
	@echo "  clean      Remove build output"
	@echo "  distclean  Also remove the CMake build directory"
