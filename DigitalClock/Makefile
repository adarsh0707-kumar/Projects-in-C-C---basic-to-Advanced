# =============================================================================
# Digital Clock System - Makefile
# =============================================================================
# Targets:
#   make            Build the application into Build/DigitalClock
#   make run        Build and run the application
#   make test       Build and run the test suite
#   make debug      Build with debug symbols and no optimisation
#   make clean      Remove build output
#   make help       List available targets
#
# Sanitizers:
#   make SANITIZE=address test
#   make SANITIZE=undefined test
#
# SANITIZE is passed straight to -fsanitize=, so any sanitizer the compiler
# supports works. It also forces -O1 -g, which keeps stack traces readable.
#
# Coverage:
#   make coverage
#
# Builds instrumented, runs the suite, and reports line coverage per file.
# Uses gcov, which ships with GCC, so there is nothing extra to install.
# =============================================================================

CXX      ?= g++
CXXSTD    = -std=c++17
WARNINGS  = -Wall -Wextra -Wpedantic -Wshadow -Wconversion
OPTIMIZE ?= -O2
SANFLAGS  =
COVFLAGS  =
# -ldl for dlopen(). Modern glibc folds it into libc, but older ones and
# the BSDs still need it, and asking for it costs nothing where it is
# already there.
LDLIBS    = -pthread $(DL_LIB)

# Not on Windows, and not needed on macOS.
ifeq ($(shell uname -s 2>/dev/null),Linux)
DL_LIB = -ldl
else
DL_LIB =
endif

ifdef SANITIZE
  SANFLAGS = -fsanitize=$(SANITIZE) -fno-omit-frame-pointer -g
  OPTIMIZE = -O1
endif

# Coverage counters are meaningless against an optimised build, because the
# optimiser reorders and merges the lines being counted.
ifdef COVERAGE
  COVFLAGS = --coverage -fprofile-abs-path -g
  OPTIMIZE = -O0
endif

CXXFLAGS  = $(CXXSTD) $(WARNINGS) $(OPTIMIZE) $(SANFLAGS) $(COVFLAGS) -IInc
LDFLAGS   = $(SANFLAGS) $(COVFLAGS)

INCDIR    = Inc
SRCDIR    = Src
TESTDIR   = Tests
OBJDIR    = Obj
BUILDDIR  = Build

TARGET      = $(BUILDDIR)/DigitalClock
TEST_TARGET = $(BUILDDIR)/DigitalClockTests

# Every source except main.cpp is shared between the app and the tests.
ALL_SOURCES  = $(wildcard $(SRCDIR)/*.cpp)
CORE_SOURCES = $(filter-out $(SRCDIR)/main.cpp,$(ALL_SOURCES))
CORE_OBJECTS = $(CORE_SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
MAIN_OBJECT  = $(OBJDIR)/main.o

TEST_SOURCES = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.cpp=$(OBJDIR)/tests/%.o)

HEADERS = $(wildcard $(INCDIR)/*.hpp)

.PHONY: all run test debug coverage plugins clean distclean help

all: $(TARGET)

$(TARGET): $(CORE_OBJECTS) $(MAIN_OBJECT) | $(BUILDDIR)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	@echo "Built $@"

# The plugins are an order-only prerequisite of the test binary rather
# than of the `test` target alone: `make coverage` builds $(TEST_TARGET)
# directly, and without this the plugin cases failed for want of their
# fixtures and took the coverage job down with them.
$(TEST_TARGET): $(CORE_OBJECTS) $(TEST_OBJECTS) | $(BUILDDIR) plugins
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	@echo "Built $@"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/tests/%.o: $(TESTDIR)/%.cpp $(HEADERS) | $(OBJDIR)/tests
	$(CXX) $(CXXFLAGS) -I$(TESTDIR) -c $< -o $@

$(OBJDIR) $(OBJDIR)/tests $(BUILDDIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

# The suite resolves Resources/ and Config/ relative to the project root.
# The bundled plugin, and a deliberately broken one the tests refuse.
# Built as shared libraries against Inc/Plugin.h alone -- they link nothing
# from the application, which is what lets a plugin be built by a different
# compiler than the clock.
PLUGIN_CFLAGS = -std=c11 -Wall -Wextra -shared -fPIC -IInc

plugins: Plugins/pomodoro.so $(BUILDDIR)/badabi.so $(BUILDDIR)/notaplugin.so

Plugins/pomodoro.so: Plugins/pomodoro.c Inc/Plugin.h
	$(CC) $(PLUGIN_CFLAGS) -o $@ $<
	@echo "Built $@"

$(BUILDDIR)/badabi.so: Tests/fixtures/badabi.c Inc/Plugin.h | $(BUILDDIR)
	$(CC) $(PLUGIN_CFLAGS) -o $@ $<

$(BUILDDIR)/notaplugin.so: Tests/fixtures/notaplugin.c | $(BUILDDIR)
	$(CC) $(PLUGIN_CFLAGS) -o $@ $<

test: $(TEST_TARGET)
	./$(TEST_TARGET)

debug: OPTIMIZE = -O0 -g
debug: clean $(TARGET)

# A separate target rather than a flag on `test`, because coverage needs a
# full instrumented rebuild -- reusing existing objects would report stale or
# missing counters.
coverage:
	$(MAKE) clean
	$(MAKE) COVERAGE=1 $(TEST_TARGET)
	./$(TEST_TARGET) > /dev/null
	@./Scripts/coverage.sh

clean:
	rm -rf $(OBJDIR) $(BUILDDIR) coverage
	rm -f *.gcov
	@echo "Removed build output"

distclean: clean
	rm -rf build cmake-build-*
	@echo "Removed CMake output"

help:
	@echo "Digital Clock System - available targets"
	@echo "  make          Build the application into $(TARGET)"
	@echo "  make run      Build and run the application"
	@echo "  make test     Build and run the test suite"
	@echo "  make debug    Build with debug symbols"
	@echo "  make coverage Report line coverage from the test suite"
	@echo "  make SANITIZE=address test    Run the suite under ASan"
	@echo "  make SANITIZE=undefined test  Run the suite under UBSan"
	@echo "  make clean    Remove build output"
	@echo "  make help     Show this message"
