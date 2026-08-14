# User Management System (C)

A CLI-based user management system written in pure C (no external
dependencies) featuring registration, login, role management, and
brute-force protection.

## Features

- **CRUD**: register, list, update role, change password, delete (soft
  delete — deleted users are marked inactive, not physically removed).
- **Password security**: passwords are never stored in plaintext. Each
  user gets a random 16-hex-char salt; the stored hash is
  `SHA-256(salt + password)`. SHA-256 is implemented from scratch in
  `src/sha256.c` — no OpenSSL or libcrypto dependency.
- **Login lockout**: after `MAX_FAILED_ATTEMPTS` (default 3) consecutive
  wrong passwords, the account is locked for `LOCKOUT_SECONDS` (default
  30s). The counter resets automatically once the lockout window
  expires, and on any successful login.
- **Binary file storage**: users are stored as fixed-size struct records
  in `data/users.dat`. Every record is O(1) addressable via
  `fseek(fp, (id-1) * sizeof(User), SEEK_SET)`, so updates (like
  incrementing failed-attempt counters) don't require rewriting the
  whole file.
- **Hidden password input**: terminal echo is disabled while typing
  passwords (via `termios`).

## Project layout

```
user_mgmt/
├── src/                 # implementation (.c) files only
│   ├── main.c           # CLI menu loop
│   ├── auth.c           # login + lockout logic
│   ├── user.c           # CRUD operations (register, update, delete, list)
│   ├── storage.c        # binary file read/write layer
│   ├── sha256.c          # self-contained SHA-256 implementation
│   └── log.c             # timestamped file logging
├── include/              # public headers (.h) for everything in src/
│   ├── auth.h / user.h / storage.h / sha256.h / log.h
├── config/
│   └── config.h          # all tunable constants (field sizes, lockout
│                          # policy, file paths) in one place
├── build/                 # build output — gitignored, created by `make`
│   ├── obj/                # intermediate .o files
│   └── bin/user_mgmt       # final executable
├── data/
│   └── users.dat            # binary user records, created at runtime
├── log/
│   └── app.log               # runtime event log, created at runtime
├── docs/
│   ├── PRD.md               # requirements, goals, non-goals, open questions
│   ├── ARCHITECTURE.md      # module design, data model, control flow
│   └── TESTING.md           # test scenarios and verified test log
├── CHANGELOG.md
├── .gitignore
├── Makefile
└── README.md
```

See `docs/PRD.md` for the full requirements this project was built
against, and `docs/ARCHITECTURE.md` for how the modules fit together.

### Runtime logging

Every registration, login (success/failure/lockout), role change,
password change, and deletion is appended to `log/app.log` as a
timestamped line, e.g.:

```
[2026-08-14 05:08:24] [INFO ] User 'adarsh' registered with id 1
[2026-08-14 05:08:24] [WARN ] Failed login attempt 1 for user 'adarsh'
[2026-08-14 05:08:24] [INFO ] Login success: user 'adarsh' (id 1)
```

This is separate from `data/users.dat` (which holds current state) —
the log is an append-only audit trail of what happened, useful for
spotting brute-force attempts after the fact.

## Build & run

```bash
make          # compiles to build/bin/user_mgmt
make run      # builds and runs
make clean    # removes build/, data/users.dat, and log/app.log
```

Requires a Linux/POSIX environment (uses `<termios.h>` and `<sys/stat.h>`).
Tested with `gcc -Wall -Wextra -std=c11` — compiles with zero warnings.

## Design notes / things to call out in an interview

- **Why binary storage over CSV**: fixed-size records let you seek
  directly to a record by id instead of rewriting the entire file for a
  single-field update (e.g., bumping `failed_attempts`). This is the
  same idea as a fixed-width row store.
- **Why salt per user**: without a salt, two users with the same
  password would have identical hashes, and precomputed rainbow tables
  become viable. A random salt defeats both.
- **Soft delete**: `active` flag instead of physically removing records
  keeps ids stable and avoids shifting every subsequent record's offset
  in the file.
- **Known limitations** (good "what would you improve" talking points):
  - No concurrent-access protection (no file locking) — fine for a
    single-user CLI tool, not for concurrent access.
  - Fixed-size `char[]` fields mean usernames/roles have a max length.
  - Lockout state is per-account, not per-IP — there's no protection
    against distributed brute force in this simple version.
  - A production system would use bcrypt/Argon2 (adaptive, slow-by-design
    hashing) instead of a single SHA-256 pass, and would add rate limiting
    at the network layer if this were exposed as a service.

## Extending it

Natural next steps if you want to grow this project further:

- Add a `role`-based permission check before allowing role updates or
  deletes (currently any menu user can do anything).
- Wrap it in a TCP server so it's a real client-server user management
  service instead of a local CLI tool.
- Swap the binary file for SQLite if you want relational queries or
  don't want to hand-roll the storage layer.
