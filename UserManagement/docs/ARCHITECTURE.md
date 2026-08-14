# Architecture

## 1. High-level design

The system is a single-process CLI application organized into layers,
each implementation file in `src/` with its public interface in the
matching `include/*.h`, plus a shared `config/config.h` for tunables.
Dependencies flow one direction only:

```
main.c   (CLI menu, I/O)
   |
   v
auth.c / user.c   (business logic: login rules, CRUD rules)
   |
   v
storage.c   (persistence: binary file read/write)
   |
   v
sha256.c   (pure utility: no dependencies on anything above)

log.c   (cross-cutting: called from auth.c/user.c to record events;
         does not depend on them, and they don't depend on its internals
         beyond the log_write() call)

config.h   (constants only, included wherever needed; no logic)
```

Nothing below a layer knows about anything above it — `storage.c`
doesn't know what a "login" is, and `sha256.c` doesn't know what a
"user" is. This keeps each file independently testable and makes it
straightforward to swap a layer later (e.g., replace `storage.c`'s
binary file backend with SQLite without touching `auth.c` or `user.c`).

### Folder layout rationale

- **`include/` separate from `src/`**: standard C convention —
  headers are the public contract of a module, implementation files
  are private. Keeping them apart also means the Makefile can pass a
  single `-Iinclude -Iconfig` and every file resolves its includes the
  same way, regardless of which `.c` file is being compiled.
- **`config/config.h`**: policy constants (lockout thresholds, field
  sizes, file paths) live in one file instead of being scattered
  across `storage.h`/`auth.c`. Changing `LOCKOUT_SECONDS` from 30 to
  60, for example, touches one line in one file.
- **`build/`**: all compiler output (`build/obj/*.o`, `build/bin/user_mgmt`)
  is generated and gitignored — the repo only tracks source. `make clean`
  simply removes this whole directory.
- **`log/`**: separate from `data/` on purpose. `data/users.dat` is
  *state* (what's true right now); `log/app.log` is an *append-only
  audit trail* (what happened over time). Mixing the two would make it
  harder to reason about either.

## 2. Module responsibilities

### `sha256.c` / `sha256.h`

Self-contained SHA-256 implementation (FIPS 180-4). Exposes a single
convenience function, `sha256_hex()`, that hashes a string and returns
a 64-char lowercase hex digest. No knowledge of users, passwords, or
files — purely a hashing primitive.

### `storage.c` / `storage.h`

Owns the `User` struct definition and all direct file I/O against
`data/users.dat`. Records are fixed-size (`sizeof(User)`), which is
what allows O(1) addressing: record `id` lives at byte offset
`(id - 1) * sizeof(User)`. Exposes CRUD-shaped primitives
(`storage_append`, `storage_update`, `storage_find_by_id`,
`storage_find_by_username`, `storage_delete`, `storage_foreach`) with
no business rules — it will happily append a user with a duplicate
username or an empty password if asked. Validation belongs to the
layer above.

### `user.c` / `user.h`

Business logic for account CRUD: enforces uniqueness of usernames,
minimum password length, salting + hashing on registration and
password change, and soft-delete semantics. This is the layer that
decides *whether* an operation is allowed; `storage.c` just decides
*how* to persist it.

### `auth.c` / `auth.h`

Business logic for login: looks up the user, checks lockout state,
verifies the password hash, and updates `failed_attempts` /
`lock_until` as a side effect of each attempt. Returns a small
`AuthResult` enum (`AUTH_OK`, `AUTH_BAD_CREDENTIALS`, `AUTH_LOCKED`,
`AUTH_NOT_FOUND`) rather than a bare int, so callers can't
misinterpret a failure reason.

### `main.c`

The only file that talks to the terminal. Renders the menu, reads
input (including hidden password input via `termios`), and dispatches
to `user_*` / `auth_*` functions. Contains no business rules of its
own.

### `log.c` / `log.h`

Cross-cutting concern, not part of the main call chain. Exposes
`log_write(level, fmt, ...)`, called from `auth.c` and `user.c` to
record registrations, logins (success/failure/lockout), role changes,
password changes, and deletions to `log/app.log` with a timestamp and
severity level. Deliberately dependency-free (only stdio/time/stdarg)
so any module can call it without creating a dependency cycle.

### `config/config.h`

Not a module with behavior — just the shared constants (`USERNAME_LEN`,
`MAX_FAILED_ATTEMPTS`, `LOCKOUT_SECONDS`, `DATA_FILE`, `LOG_FILE`, etc.)
that `storage.h`, `auth.c`, and `log.c` all include. Centralizing these
means a policy change never requires hunting across multiple files.

## 3. Data model

```c
typedef struct {
    int      id;                 // 1-indexed, assigned at registration
    char     username[32];
    char     salt[17];           // 16 hex chars + null
    char     password_hash[65];  // sha256(salt + password), 64 hex chars + null
    char     role[16];
    int      failed_attempts;
    time_t   lock_until;         // 0 = not locked, else unix timestamp
    int      active;             // 0 = soft-deleted
} User;
```

Every record is a fixed size, which is the load-bearing design choice
for the storage layer (see below).

## 4. Storage design

**Format**: flat binary file, `data/users.dat`, containing consecutive
`fwrite`s of the `User` struct — no delimiters, no headers, no length
prefixes needed, because every record is the same size.

**Why binary over CSV/text**: a text format (CSV, JSON-lines) requires
either rewriting the entire file to update one field, or a more complex
format with padding/indexing to support seeking. With fixed-size binary
records, updating `failed_attempts` on a single login attempt is:

```c
fseek(fp, (id - 1) * sizeof(User), SEEK_SET);
fwrite(&updated_user, sizeof(User), 1, fp);
```

— an O(1) operation regardless of how many users exist.

**Why soft delete**: physically removing a record would shift every
subsequent record's byte offset, breaking the `id -> offset` formula
for every user after the deleted one. Soft delete (`active = 0`) keeps
offsets stable at the cost of leaving inactive records on disk
(acceptable for this project's scale; see `docs/PRD.md` for the
data-scale non-goals).

**Why not SQLite for v1**: the goal of this project is to demonstrate
manual file I/O and struct-based storage as a C systems exercise. This
is a `docs/PRD.md` v2 candidate.

## 5. Security design

- **Salting**: each user gets a random 16-hex-char salt (`rand()`,
  seeded once from `time(NULL)` in `main()`). Salting means two users
  with the same password get different hashes, which defeats
  precomputed rainbow-table attacks.
- **Hashing**: `SHA-256(salt + password)`. Implemented from scratch
  (`sha256.c`) rather than depending on OpenSSL, per this project's
  no-external-dependencies goal.
- **Lockout**: after `MAX_FAILED_ATTEMPTS` (3) consecutive wrong
  passwords, `lock_until` is set to `now + LOCKOUT_SECONDS` (30s).
  Login attempts during that window return `AUTH_LOCKED` without even
  checking the password — this avoids leaking whether a guessed
  password would have been correct while locked.
- **Hidden input**: `main.c` disables terminal echo (`termios`,
  clearing `ECHO`) while reading passwords.

See `docs/PRD.md` §9 for the known limitations of this approach
(no adaptive hashing, no file locking, no per-IP protection).

## 6. Control flow: login example

```
main.c: do_login()
  -> read username, hidden-read password
  -> auth_login(username, password, &id)
       storage.c: storage_find_by_username()
         -> fopen, linear scan for matching active username
       [check lock_until vs now]
       [if unlocked/expired: reset failed_attempts]
       sha256.c: sha256_hex(salt + password)
       [compare computed hash to stored hash]
       [update failed_attempts / lock_until]
       storage.c: storage_update()
         -> fseek to (id-1)*sizeof(User), fwrite updated record
  -> print result based on returned AuthResult
```

## 7. Known limitations affecting architecture

- **No authorization layer**: `main.c` allows any logged-in-or-not user
  to call `user_update_role` / `user_delete` on any id. There's
  currently no session concept tying a menu action to "who is
  currently logged in." A v2 would need `main.c` to track a "current
  session user" and check `role == "admin"` before allowing
  administrative actions (see `docs/PRD.md` Open Questions).
- **No concurrency control**: `storage.c` does not lock the file, so
  two processes writing simultaneously could interleave writes and
  corrupt records. Fine for the current single-process CLI; would need
  `flock()` or a proper database before any multi-process/networked
  use.
