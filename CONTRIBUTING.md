# Contributing to Projects-in-C-C---basic-to-Advanced

Thanks for your interest in contributing to this project! This repo tracks a scientific calculator built incrementally in C/C++, organized as a series of phases (basic arithmetic through advanced features like matrix evaluation, statistics, and plotting).

## Project Structure

- Each feature area lives in its own module (e.g. `angle_mode`, `units`, `complex_eval`, `matrix_eval`, `stats`, `base`, `plot`).
- Development proceeds **phase by phase** — each phase adds a well-defined, self-contained piece of functionality.
- Tests live alongside the code and are organized per-module (e.g. `test_angle_mode.cpp`, `test_units.cpp`, `test_stats.cpp`, `test_base.cpp`, `test_complex.cpp`, `test_matrix.cpp`).

## Development Workflow

1. **Fork and clone** the repository.
2. **Work one phase at a time.** Don't mix multiple phases or unrelated features in a single change — this keeps history clean and makes review/debugging straightforward.
3. **Build and test locally** before committing:
   ```bash
   make
   make test
   ```
   (Adjust these commands if your local setup uses a different build script.)
4. **Write or update tests** for any new module or behavior. All new functionality should ship with corresponding unit tests — the project currently maintains full passing coverage across all test suites, and new code shouldn't lower that bar.
5. **Commit with a clear, phase-scoped message**, e.g.:
   ```
   Phase 22: Add polynomial root-finding module

   - Implements Newton-Raphson solver in poly_eval
   - Adds test_poly.cpp with 12 test cases
   ```
6. **Push and open a pull request** against `main`, describing what phase/feature the PR covers and what was tested.

## Coding Guidelines

- Match the existing code style already used in the module you're editing (naming conventions, indentation, header/source split).
- Keep modules loosely coupled — a new feature module shouldn't require invasive changes to unrelated modules.
- Prefer clear, readable C/C++ over clever one-liners; this is a learning-oriented, incrementally built project.
- Avoid introducing external dependencies unless there's a strong reason — the project favors a minimal, self-contained build.

## Reporting Issues

If you find a bug or want to propose a new phase/feature, open an issue describing:
- What you expected to happen
- What actually happened (with steps to reproduce, if applicable)
- Which module/phase it relates to, if known

## Pull Request Checklist

- [ ] Change is scoped to a single phase or logical feature
- [ ] Code builds cleanly with no new warnings
- [ ] New/changed behavior has test coverage
- [ ] All existing tests still pass
- [ ] Commit messages are clear and reference the relevant phase

## Questions

If anything about the project structure or workflow is unclear, feel free to open an issue and ask before starting work — happy to clarify.
