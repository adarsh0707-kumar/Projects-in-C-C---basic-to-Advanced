
# Minimal Doxygen configuration for the Advanced Scientific Calculator.

# Every option not listed here uses Doxygen's built-in default.

# Usage:

# sudo apt install doxygen graphviz   # graphviz is optional, for diagrams

# make docs                            # from the project root

# open docs/html/index.html

PROJECT_NAME           = "Advanced Scientific Calculator"
PROJECT_BRIEF           = "A modular scientific calculator engine in C/C++"

# Source is documented with /** ... */ + @brief/@param/@return tags.

INPUT                   = Inc Src
FILE_PATTERNS            = *.h *.hpp *.c *.cpp
RECURSIVE                = NO

# Output alongside the rest of the docs/ directory.

OUTPUT_DIRECTORY         = docs

GENERATE_HTML            = YES
GENERATE_LATEX           = NO

# Document every declaration, even ones without a doc comment, so

# nothing silently falls through the cracks as the project grows.

EXTRACT_ALL               = YES
EXTRACT_STATIC            = YES

# Call/caller graphs need Graphviz (`dot`); harmless if it's not

# installed -- Doxygen just skips them with a warning.

HAVE_DOT                  = YES
CALL_GRAPH                = YES
CALLER_GRAPH              = YES

# This is a small, single-module project -- one page per file is

# more useful here than a huge alphabetical member index.

SORT_MEMBER_DOCS          = NO

QUIET                     = YES
WARN_IF_UNDOCUMENTED      = YES
