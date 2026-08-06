# Contributing

Thanks for your interest in the Digital Clock System.

## Getting started

```bash
git clone <repository-url>
cd DigitalClock
make          # build
make test     # run the suite
make run      # try it
```

You need a C++17 compiler (GCC 8+, Clang 7+, MSVC 2019+) and `make` or
CMake 3.16+. There are no other dependencies, and adding one needs a good
reason — the standard-library-only constraint is deliberate.

## Before you open a pull request

1. `make test` passes — all 60 tests.
2. The build is warning-free. The project compiles with `-Wall -Wextra
   -Wpedantic -Wshadow -Wconversion`; keep it at zero warnings.
3. New behaviour comes with a test.
4. New public methods carry Doxygen comments.

Also worth running:

```bash
make debug                                  # -O0 -g
cmake -S . -B build && ctest --test-dir build --output-on-failure
```

## Code style

Follow what is already there. In short:

- Classes `PascalCase`, functions and variables `camelCase`, constants
  `UPPER_SNAKE_CASE`.
- Headers `.hpp` in `Inc/`, sources `.cpp` in `Src/`, one class per pair.
- Allman braces, four-space indent, no tabs.
- Mark methods `const` where they are; pass strings by `const &`.
- Prefer returning a status over throwing. The application is expected to
  survive missing files, bad configuration and an unwritable log.

Comments should explain *why*, not restate the code. If a line needs a comment
to say what it does, the line usually wants rewriting instead.

## Architecture

The layering is the thing most worth preserving:

```text
Presentation   Console · Display · Screen · Banner · StatusBar
Business logic Clock · Date · TimeFormatter
Service        ConfigurationManager · ThemeManager · Logger
               ResourceManager · Utility
```

A layer may use the ones below it, never the ones above. Two rules follow:

- **Only `Console` touches the terminal.** No other class writes escape
  sequences or reads keys. This is what keeps a future GUI port feasible.
- **Only `TimeFormatter` formats.** `Clock` and `Date` expose values; the
  presentation layer receives finished strings.

`Application` owns the component graph; `main()` only parses arguments.

## Tests

Tests live in `Tests/`, one file per component, using the small harness in
`Tests/TestFramework.hpp`:

```cpp
TEST_CASE(TC_003, "Verify current system time")
{
    Clock clock;
    CHECK_TRUE(clock.update());
    CHECK_EQ(clock.getTime24(), std::string("09:05:03"));
}
```

Available assertions: `CHECK_TRUE`, `CHECK_FALSE`, `CHECK_EQ`, `CHECK_NE`,
`CHECK_CONTAINS`, `CHECK_NOT_CONTAINS`, `FAIL_TEST`.

Identifiers: use `TC_nnn` for cases traced to `Docs/Testing_Report.md`, and
`UT_nnn` for supporting unit tests. Register a new file by adding it to
`CMakeLists.txt`; the Makefile globs `Tests/*.cpp` automatically.

Two things to keep in mind:

- Tests run from the project root so they can find `Config/` and `Resources/`.
- Anything that drives the presentation layer must wrap the call in
  `TestFramework::OutputCapture`, or escape sequences will scramble the report.
  Use `TestFramework::writeTempFile()` for fixtures; the scratch directory is
  cleaned up automatically.

## Commit messages

Conventional commits, matching the existing history:

```text
feat(clock): add 12-hour format support
fix(config): reject refresh intervals outside the valid range
docs(readme): document the theme file format
test(logger): cover the unwritable log directory case
refactor(screen): centre banner artwork as a block
```

## Reporting bugs

Please include your OS and compiler version, the steps to reproduce, what you
expected, what happened, and the relevant part of `Logs/application.log`.
Setting `LogLevel=DEBUG` in `Config/config.ini` gives more detail.

## License

Contributions are licensed under the MIT License; see [LICENSE](LICENSE).
