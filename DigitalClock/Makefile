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
# =============================================================================

CXX      ?= g++
CXXSTD    = -std=c++17
WARNINGS  = -Wall -Wextra -Wpedantic -Wshadow -Wconversion
OPTIMIZE ?= -O2
SANFLAGS  =
LDLIBS    = -pthread

ifdef SANITIZE
  SANFLAGS = -fsanitize=$(SANITIZE) -fno-omit-frame-pointer -g
  OPTIMIZE = -O1
endif

CXXFLAGS  = $(CXXSTD) $(WARNINGS) $(OPTIMIZE) $(SANFLAGS) -IInc
LDFLAGS   = $(SANFLAGS)

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

.PHONY: all run test debug clean distclean help

all: $(TARGET)

$(TARGET): $(CORE_OBJECTS) $(MAIN_OBJECT) | $(BUILDDIR)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	@echo "Built $@"

$(TEST_TARGET): $(CORE_OBJECTS) $(TEST_OBJECTS) | $(BUILDDIR)
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
test: $(TEST_TARGET)
	./$(TEST_TARGET)

debug: OPTIMIZE = -O0 -g
debug: clean $(TARGET)

clean:
	rm -rf $(OBJDIR) $(BUILDDIR)
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
	@echo "  make SANITIZE=address test    Run the suite under ASan"
	@echo "  make SANITIZE=undefined test  Run the suite under UBSan"
	@echo "  make clean    Remove build output"
	@echo "  make help     Show this message"
