# ==================================================================
# Calculator - Makefile
#
# Build types (select with BUILD=<type>, default is debug):
#   debug    -g -O0, no optimization, best for local development
#   release  -O2 -DNDEBUG, optimized binary for actual use
#   asan     debug build + AddressSanitizer + UndefinedBehaviorSanitizer
#            (catches buffer overruns, use-after-free, UB, etc.)
#   ubsan    debug build + UndefinedBehaviorSanitizer only
#
# Each build type gets its own object directory (Obj/<type>) so
# switching between them never links stale objects from another
# configuration.
#
# Header dependencies are tracked automatically via -MMD -MP, so a
# change to any header correctly triggers a rebuild of everything
# that includes it -- no manually-maintained header list to forget
# to update.
#
# Common commands:
#   make                 -> debug build
#   make release         -> optimized build
#   make asan && make run BUILD=asan   -> run under sanitizers
#   make test            -> build and run the unit test suite
#   make docs            -> generate Doxygen HTML docs (see docs/Doxyfile)
#   make clean           -> remove the current BUILD's objects/binaries
#   make distclean       -> remove every build type's objects/binaries
# ==================================================================

CC  = gcc
CXX = g++

SRC_DIR  = Src
INC_DIR  = Inc
TEST_DIR = Tests
DOCS_DIR = docs
GUI_DIR  = Gui

TARGET      = calculator
TEST_TARGET = run_tests
GUI_TARGET  = calculator-gui

# ------------------------------------------------------------------
# Build type configuration
# ------------------------------------------------------------------
BUILD ?= debug

ifeq ($(BUILD),debug)
    OPT_FLAGS = -g -O0 -DDEBUG
    SAN_FLAGS =
else ifeq ($(BUILD),release)
    OPT_FLAGS = -O2 -DNDEBUG
    SAN_FLAGS =
else ifeq ($(BUILD),asan)
    OPT_FLAGS = -g -O0 -fno-omit-frame-pointer
    SAN_FLAGS = -fsanitize=address,undefined
else ifeq ($(BUILD),ubsan)
    OPT_FLAGS = -g -O0 -fno-omit-frame-pointer
    SAN_FLAGS = -fsanitize=undefined
else
    $(error Unknown BUILD '$(BUILD)'. Use one of: debug, release, asan, ubsan)
endif

OBJ_DIR      = Obj/$(BUILD)
TEST_OBJ_DIR = Obj/$(BUILD)-tests
GUI_OBJ_DIR  = Obj/$(BUILD)-gui

WARN_FLAGS = -Wall -Wextra
DEPFLAGS   = -MMD -MP

CFLAGS   = $(WARN_FLAGS) -std=c11   -I$(INC_DIR) $(OPT_FLAGS) $(SAN_FLAGS) $(DEPFLAGS)
CXXFLAGS = $(WARN_FLAGS) -std=c++17 -I$(INC_DIR) $(OPT_FLAGS) $(SAN_FLAGS) $(DEPFLAGS)
LDFLAGS  = -lm $(SAN_FLAGS)

# ------------------------------------------------------------------
# Phase 31 -- GUI (Qt6). Only resolved/used by the gui/run-gui
# targets below; every other target is unaffected whether or not Qt
# is installed.
# ------------------------------------------------------------------
QT_CFLAGS := $(shell pkg-config --cflags Qt6Widgets 2>/dev/null)
QT_LIBS   := $(shell pkg-config --libs Qt6Widgets 2>/dev/null)
# A system's default `moc` on PATH may belong to Qt5 even when Qt6
# headers/libs are what pkg-config resolves above (that's the case on
# this machine) -- using a mismatched moc produces meta-object code
# that fails to compile against the newer headers. qmake6's
# QT_HOST_LIBEXECS reliably points at the matching Qt6 moc.
MOC := $(shell qmake6 -query QT_HOST_LIBEXECS 2>/dev/null)/moc
# Same story for rcc, which compiles Gui/resources.qrc (the app icon)
# into a C++ source file.
RCC := $(shell qmake6 -query QT_HOST_LIBEXECS 2>/dev/null)/rcc

# ------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/error.c \
       $(SRC_DIR)/stack.c \
       $(SRC_DIR)/tokenizer.c \
       $(SRC_DIR)/postfix.c \
       $(SRC_DIR)/evaluate.c \
       $(SRC_DIR)/parser.c \
       $(SRC_DIR)/validator.c \
       $(SRC_DIR)/history.c \
       $(SRC_DIR)/memory.c \
       $(SRC_DIR)/variables.c \
       $(SRC_DIR)/functions.c \
       $(SRC_DIR)/function_info.c \
       $(SRC_DIR)/constants.c \
       $(SRC_DIR)/angle_mode.c \
       $(SRC_DIR)/units.c \
       $(SRC_DIR)/stats.c \
       $(SRC_DIR)/base.c \
       $(SRC_DIR)/plot.c

# C++ sources, isolated to the complex-number / matrix features --
# see complex_eval.h / matrix_eval.h for the C-facing interfaces.
CXX_SRCS = $(SRC_DIR)/Complex.cpp \
           $(SRC_DIR)/complex_eval.cpp \
           $(SRC_DIR)/Matrix.cpp \
           $(SRC_DIR)/matrix_eval.cpp

OBJS     = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CXX_OBJS = $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# ------------------------------------------------------------------
# Phase 20 -- Unit tests
#
# Tests link the real module .c files directly (not through the
# library), swapping in Tests/test_main.c in place of Src/main.c so
# there's exactly one main(). This keeps the tests exercising the
# actual production code, not a reimplementation of it.
# ------------------------------------------------------------------
TEST_SRCS = $(TEST_DIR)/test_main.c \
            $(TEST_DIR)/test_error.c \
            $(TEST_DIR)/test_stack.c \
            $(TEST_DIR)/test_tokenizer.c \
            $(TEST_DIR)/test_validator.c \
            $(TEST_DIR)/test_parser.c \
            $(TEST_DIR)/test_postfix.c \
            $(TEST_DIR)/test_functions.c \
            $(TEST_DIR)/test_variables.c \
            $(TEST_DIR)/test_history.c \
            $(TEST_DIR)/test_memory.c \
            $(TEST_DIR)/test_units.c \
            $(TEST_DIR)/test_stats.c \
            $(TEST_DIR)/test_base.c \
            $(TEST_DIR)/test_plot.c \
            $(TEST_DIR)/test_complex.c \
            $(TEST_DIR)/test_matrix.c

# Every module under test, EXCEPT main.c (Tests/test_main.c supplies
# main() instead).
TESTED_SRCS = $(filter-out $(SRC_DIR)/main.c,$(SRCS))

TEST_OBJS       = $(TEST_SRCS:$(TEST_DIR)/%.c=$(TEST_OBJ_DIR)/test_%.o)
TESTED_OBJS     = $(TESTED_SRCS:$(SRC_DIR)/%.c=$(TEST_OBJ_DIR)/%.o)
# test_complex.c/test_matrix.c call the extern "C" entry points into
# Complex.cpp/Matrix.cpp/complex_eval.cpp/matrix_eval.cpp, so those
# four C++ objects need to be in the test binary too.
TESTED_CXX_OBJS = $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(TEST_OBJ_DIR)/%.o)

# ------------------------------------------------------------------
# Phase 31 -- GUI (Qt6)
#
# Gui/*.cpp calls straight into the existing C engine (Inc/
# calculator.h etc.) through an extern "C" include block -- see
# Gui/MainWindow.cpp -- instead of duplicating any parsing/evaluation
# logic. The engine objects it links against are the same $(OBJS)/
# $(CXX_OBJS) the CLI binary uses (minus main.o, which Gui/main.cpp
# replaces with its own), so `make gui` builds/reuses them from
# $(OBJ_DIR) rather than recompiling the engine a third time.
# ------------------------------------------------------------------
GUI_SRCS      = $(GUI_DIR)/MainWindow.cpp $(GUI_DIR)/PlotWidget.cpp $(GUI_DIR)/main.cpp
# PlotWidget.hpp has no Q_OBJECT (no signals/slots of its own -- see
# Gui/PlotWidget.hpp), so it needs no moc step, unlike MainWindow.hpp.
MOC_HEADERS   = $(GUI_DIR)/MainWindow.hpp
MOC_GEN_SRCS  = $(MOC_HEADERS:$(GUI_DIR)/%.hpp=$(GUI_OBJ_DIR)/moc_%.cpp)

# The app icon, embedded rather than read from disk -- see Gui/resources.qrc.
QRC_FILES     = $(GUI_DIR)/resources.qrc
QRC_GEN_SRCS  = $(QRC_FILES:$(GUI_DIR)/%.qrc=$(GUI_OBJ_DIR)/qrc_%.cpp)

GUI_OBJS = $(GUI_SRCS:$(GUI_DIR)/%.cpp=$(GUI_OBJ_DIR)/%.o) \
           $(MOC_GEN_SRCS:$(GUI_OBJ_DIR)/%.cpp=$(GUI_OBJ_DIR)/%.o) \
           $(QRC_GEN_SRCS:$(GUI_OBJ_DIR)/%.cpp=$(GUI_OBJ_DIR)/%.o)

# Rendered from Gui/icon.svg by `make icons`. Committed, so a normal
# build needs neither rsvg-convert nor a regeneration step.
ICON_SVG   = $(GUI_DIR)/icon.svg
ICON_SIZES = 16 24 32 48 64 128 256
ICON_PNGS  = $(foreach s,$(ICON_SIZES),$(GUI_DIR)/icons/calculator-$(s).png)

ENGINE_OBJS_FOR_GUI = $(filter-out $(OBJ_DIR)/main.o,$(OBJS)) $(CXX_OBJS)

# ------------------------------------------------------------------
# Primary targets
# ------------------------------------------------------------------
.PHONY: all debug release asan ubsan run test docs clean distclean rebuild gui run-gui icons \
        install-desktop uninstall-desktop

all: $(TARGET)

debug:
	$(MAKE) BUILD=debug all

release:
	$(MAKE) BUILD=release all

asan:
	$(MAKE) BUILD=asan all

ubsan:
	$(MAKE) BUILD=ubsan all

# Link (uses g++ since the C++ objects need libstdc++)
$(TARGET): $(OBJS) $(CXX_OBJS)
	$(CXX) -o $@ $(OBJS) $(CXX_OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

# ------------------------------------------------------------------
# GUI (Qt6) -- see the "Phase 31" comment above for how this reuses
# the engine objects. Requires Qt6Widgets (pkg-config); every other
# target works with or without Qt installed.
# ------------------------------------------------------------------
gui: $(OBJS) $(CXX_OBJS) $(GUI_TARGET)

$(GUI_TARGET): $(GUI_OBJS)
	$(CXX) -o $@ $(ENGINE_OBJS_FOR_GUI) $(GUI_OBJS) $(LDFLAGS) $(QT_LIBS)

# -fPIC (compile-only; moc, below, doesn't take it -- it parses -f as
# its own "-f<file>" option): omitting it fails at link time with
# "copy relocation against non-copyable protected symbol ... QObject".
# Qt's own qmake/CMake tooling always compiles against Qt with -fPIC
# for exactly this reason, so this matches that convention.
$(GUI_OBJ_DIR)/%.o: $(GUI_DIR)/%.cpp | $(GUI_OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -fPIC -c $< -o $@

$(GUI_OBJ_DIR)/moc_%.cpp: $(GUI_DIR)/%.hpp | $(GUI_OBJ_DIR)
	$(MOC) $(QT_CFLAGS) $< -o $@

$(GUI_OBJ_DIR)/moc_%.o: $(GUI_OBJ_DIR)/moc_%.cpp
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -fPIC -c $< -o $@

# rcc reads the <file> paths in the .qrc relative to the .qrc itself,
# so it runs from $(GUI_DIR); the generated .cpp still lands in the
# build directory like every other generated source.
$(GUI_OBJ_DIR)/qrc_%.cpp: $(GUI_DIR)/%.qrc $(ICON_PNGS) | $(GUI_OBJ_DIR)
	$(RCC) $< -o $@

$(GUI_OBJ_DIR)/qrc_%.o: $(GUI_OBJ_DIR)/qrc_%.cpp
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -fPIC -c $< -o $@

$(GUI_OBJ_DIR):
	mkdir -p $(GUI_OBJ_DIR)

run-gui: gui
	./$(GUI_TARGET)

# ------------------------------------------------------------------
# Icons -- only needed after editing Gui/icon.svg. The PNGs are
# committed, so this is not part of any build target; it needs
# rsvg-convert (librsvg), which the build itself does not.
# ------------------------------------------------------------------
icons:
	@command -v rsvg-convert >/dev/null || \
		{ echo "rsvg-convert not found -- install librsvg to regenerate icons"; exit 1; }
	@mkdir -p $(GUI_DIR)/icons
	@for s in $(ICON_SIZES); do \
		rsvg-convert -w $$s -h $$s $(ICON_SVG) -o $(GUI_DIR)/icons/calculator-$$s.png && \
		echo "  generated $(GUI_DIR)/icons/calculator-$$s.png"; \
	done

# ------------------------------------------------------------------
# Desktop entry -- the icon is compiled into the binary, so the window
# already has it while running; this is what additionally puts the app
# in the desktop's launcher/menu. Per-user (no root), and undone by
# `make uninstall-desktop`. Exec is substituted with the absolute path
# of the binary in this build tree, so it works without installing the
# binary anywhere.
# ------------------------------------------------------------------
DESKTOP_DIR   = $(HOME)/.local/share/applications
HICOLOR_DIR   = $(HOME)/.local/share/icons/hicolor
DESKTOP_ENTRY = $(DESKTOP_DIR)/calculator-gui.desktop

install-desktop: gui
	@mkdir -p $(DESKTOP_DIR)
	@sed 's|@EXEC@|$(CURDIR)/$(GUI_TARGET)|' $(GUI_DIR)/calculator-gui.desktop.in > $(DESKTOP_ENTRY)
	@for s in $(ICON_SIZES); do \
		mkdir -p $(HICOLOR_DIR)/$${s}x$${s}/apps; \
		cp $(GUI_DIR)/icons/calculator-$$s.png $(HICOLOR_DIR)/$${s}x$${s}/apps/calculator-gui.png; \
	done
	@mkdir -p $(HICOLOR_DIR)/scalable/apps
	@cp $(ICON_SVG) $(HICOLOR_DIR)/scalable/apps/calculator-gui.svg
	@command -v update-desktop-database >/dev/null && update-desktop-database $(DESKTOP_DIR) || true
	@command -v gtk-update-icon-cache >/dev/null && gtk-update-icon-cache -qtf $(HICOLOR_DIR) || true
	@echo "Installed $(DESKTOP_ENTRY) -> $(CURDIR)/$(GUI_TARGET)"

uninstall-desktop:
	@rm -f $(DESKTOP_ENTRY)
	@for s in $(ICON_SIZES); do \
		rm -f $(HICOLOR_DIR)/$${s}x$${s}/apps/calculator-gui.png; \
	done
	@rm -f $(HICOLOR_DIR)/scalable/apps/calculator-gui.svg
	@command -v update-desktop-database >/dev/null && update-desktop-database $(DESKTOP_DIR) || true
	@command -v gtk-update-icon-cache >/dev/null && gtk-update-icon-cache -qtf $(HICOLOR_DIR) || true
	@echo "Removed the desktop entry and its icons"

# ------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Linked with g++ (not gcc) because $(TESTED_CXX_OBJS) pulls in
# libstdc++ (std::vector, std::string, exceptions) -- same reasoning
# as the main $(TARGET) link rule above.
$(TEST_TARGET): $(TEST_OBJS) $(TESTED_OBJS) $(TESTED_CXX_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(TEST_OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -I$(TEST_DIR) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(TEST_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR):
	mkdir -p $(TEST_OBJ_DIR)

# ------------------------------------------------------------------
# Docs (Doxygen) -- see docs/Doxyfile
# ------------------------------------------------------------------
docs:
	doxygen $(DOCS_DIR)/Doxyfile

# ------------------------------------------------------------------
# Housekeeping
# ------------------------------------------------------------------
clean:
	rm -rf $(OBJ_DIR) $(TEST_OBJ_DIR) $(GUI_OBJ_DIR) $(TARGET) $(TEST_TARGET) $(GUI_TARGET)

distclean:
	rm -rf Obj $(TARGET) $(TEST_TARGET) $(GUI_TARGET)
	rm -rf $(DOCS_DIR)/html $(DOCS_DIR)/latex

rebuild: clean all

# Pull in auto-generated header dependencies, if any exist yet.
-include $(wildcard $(OBJ_DIR)/*.d)
-include $(wildcard $(TEST_OBJ_DIR)/*.d)
-include $(wildcard $(GUI_OBJ_DIR)/*.d)
