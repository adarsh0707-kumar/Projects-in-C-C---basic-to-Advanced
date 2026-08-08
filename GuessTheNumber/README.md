# Guess The Number

A small terminal game in C11. It picks a number between 1 and 100 and tells
you whether each guess is too high or too low, until you find it.

No dependencies — the standard library only.

```text
Welcome to Guess The Number Game!

Please enter your guess between 1 and 100: 50
Your guess is too high. Try again!

Please enter your guess between 1 and 100: 25
Your guess is too low. Try again!

Please enter your guess between 1 and 100: 37

Congratulations!! You guessed the correct number: 37 in 3 attempts.

Thank you for playing the game!
```

---

## Building

Requires a C11 compiler and either `make` or CMake 3.16+.

```bash
make          # builds Build/guessthenumber
make run      # builds and plays
make test     # builds and runs the test suite
make clean    # removes build output
make help     # lists all targets
```

With CMake:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## Layout

```text
GuessTheNumber/
├── Build/     Compiled output (generated)
├── Inc/       Game.h  - the rules
├── Obj/       Object files (generated)
├── Src/       Game.c  - the rules
│              main.c  - printing and looping
├── Tests/     test_game.c
├── CMakeLists.txt
└── Makefile
```

The rules live apart from the printing, which is the only reason the game can
be tested at all: `gtn_read_guess()` takes a `FILE *` rather than reading
`stdin` directly, so a test can hand it a stream of deliberate nonsense.

---

## The bug this was rewritten for

The first version was 34 lines and read guesses like this:

```c
scanf("%d", &guess);
```

Type a letter and the game printed `Your guess is too low.` forever.

`scanf` with `%d` does not consume input it cannot parse. The `x` stayed in
the buffer, so the next call failed on the same character; `guess` kept
whatever value it had; and the loop condition `while (random != guess)` was
never satisfied. Nothing in the program was individually wrong — the loop,
the comparison and the prompt were all fine — but together they spun.

Measured before the rewrite: **36.6 million lines of output in three seconds**
from a single stray keystroke, still going when the timeout killed it.

The fix reads a whole line and parses it, so a refused line is *gone* and a
retry actually retries. It also distinguishes three outcomes that the old
code collapsed into one:

| Outcome | What it means | What the game does |
| ------- | ------------- | ------------------ |
| `GTN_READ_OK` | A number in range | Judge the guess |
| `GTN_READ_INVALID` | Not a number, or out of range | Say so, ask again |
| `GTN_READ_EOF` | Input has ended | Stop |

That last one matters too: the old version could not tell "nonsense" from
"no more input", so piping it an empty file span just as hard.

---

## Tests

```bash
make test
```

50 checks across 8 cases, covering the comparison, the range, the number
picker and — mostly — the reader, since that is where the defect was.

`TC-006` is the regression test: it feeds three bad lines followed by a good
one and requires the good one to be reached. If a bad line were ever left in
the buffer again, the reader would keep returning the same failure and the
test would fail rather than hang.

CI runs the suite on Linux, macOS and Windows, builds with `-Werror` under
five warning flags, runs it under AddressSanitizer and UndefinedBehavior‑
Sanitizer, and separately drives the real binary with rubbish input and a
timeout — because the original failure was a hang of the whole program, not
a wrong return value.

---

## License

MIT — see the repository root.
