# Testing

This project currently uses manual/scripted functional testing (no unit
test framework wired in yet — see "Future: automated tests" below).
Each scenario below was run against a clean build and verified to
behave as described.

## How to reset to a clean state

```bash
make clean   # removes binaries, object files, and data/users.dat
make         # rebuilds
```

## Test scenarios

### 1. Registration

| Case               | Steps                                          | Expected result                                    |
| ------------------ | ---------------------------------------------- | -------------------------------------------------- |
| Happy path         | Register`adarsh` / `secret123` / `admin` | User created with id 1, listed as`active`        |
| Duplicate username | Register`adarsh` again                       | Rejected: "username already exists"                |
| Password too short | Register with a 3-char password                | Rejected: "invalid username or password too short" |

### 2. Login

| Case                              | Steps                                                            | Expected result                                                                                                      |
| --------------------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| Correct credentials               | Login`adarsh` / `secret123`                                  | "Login successful"                                                                                                   |
| Wrong password (1st, 2nd attempt) | Login with wrong password twice                                  | "incorrect username or password" each time, account stays unlocked                                                   |
| Wrong password (3rd attempt)      | Login with wrong password a 3rd consecutive time                 | Account becomes locked; message: "Account locked due to too many failed attempts"                                    |
| Correct password while locked     | Immediately try the*correct* password                          | Still rejected with lockout message — lockout blocks all attempts, including correct ones, until the window expires |
| Login after cooldown              | Wait`LOCKOUT_SECONDS` (30s), then log in with correct password | Succeeds;`failed_attempts` resets to 0                                                                             |
| Nonexistent user                  | Login with a username that was never registered                  | "no such user"                                                                                                       |

### 3. CRUD

| Case                     | Steps                                              | Expected result                                                                                                           |
| ------------------------ | -------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------- |
| List users               | Choice`3`                                        | Table of all active users with id, username, role, status                                                                 |
| Update role              | Choice`4`, valid id, new role                    | "Role updated"; reflected in subsequent list                                                                              |
| Update role, bad id      | Choice`4`, nonexistent id                        | "User not found"                                                                                                          |
| Change password          | Choice`5`, valid id, new password                | "Password updated"; old password no longer works, new one does;`failed_attempts`/`lock_until` reset                   |
| Delete user              | Choice`6`, valid id                              | "User N deleted"; user no longer appears in list (soft-deleted,`active = 0`)                                            |
| Re-register after delete | Register the same username again after deleting it | Succeeds with a new id (soft-delete frees the username since`storage_find_by_username` only matches `active` records) |

## Test log (last verified run)

The following sequence was scripted via piped stdin against the compiled
binary and manually reviewed:

1. Register `adarsh` / `secret123` / `admin` → id 1, confirmed via list.
2. Login with correct password → success.
3. Login with wrong password x2 → both rejected, not locked.
4. Login with wrong password x3rd → locked.
5. Login with *correct* password while locked → still rejected (lockout, not credential check).
6. Waited 31s (past the 30s lockout window), logged in with correct password → success.
7. Updated role to `superadmin` → confirmed via list.
8. Deleted user id 1 → confirmed list is empty.
9. Re-registered `adarsh` → succeeded with id 2 (ids are not reused/decremented).

All steps passed as expected. `make` produces zero warnings under
`-Wall -Wextra -std=c11`.

## Future: automated tests

Not yet implemented. If this project grows, the natural next step is a
small test harness that:

- Builds `user.c`, `auth.c`, `storage.c`, `sha256.c` against a test
  `main()` (e.g., using a lightweight C test framework like
  [Unity](https://github.com/ThrowTheSwitch/Unity) or hand-rolled
  `assert()`-based tests).
- Points `DATA_FILE` at a temp file per test run so tests don't clobber
  real data or each other.
- Covers: SHA-256 known-answer tests (NIST test vectors), storage
  round-trip (write then read back a record), and auth lockout timing
  using an injectable clock instead of real `sleep()`.

This mirrors the incomplete Phase 21 test-coverage work from the
scientific calculator project (`test_complex.cpp` / `test_matrix.cpp`
pattern) — same idea, applied here.
