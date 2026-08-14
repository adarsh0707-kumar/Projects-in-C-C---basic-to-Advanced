# PRD: User Management System (C)

## 1. Overview

A lightweight, dependency-free user management system implemented in C,
providing account registration, authentication, and administrative CRUD
operations via a command-line interface. Designed as a self-contained
systems-programming project demonstrating secure credential handling,
binary file I/O, and structured C application design.

## 2. Problem Statement

Many introductory C projects handle credentials insecurely (plaintext
passwords, no lockout protection) or rely on external databases that
obscure the underlying systems concepts. This project implements user
management primitives — authentication, password hashing, brute-force
protection, persistent storage — from first principles in C, with no
external libraries beyond the C standard library and POSIX headers.

## 3. Goals

- Provide secure account registration and login via a CLI.
- Store credentials safely: no plaintext passwords on disk.
- Protect against brute-force login attempts via lockout.
- Persist data across program runs without a database dependency.
- Keep the codebase modular enough to extend (e.g., roles, network layer)
  without a rewrite.

## 4. Non-Goals

- Not a multi-user networked service (single local CLI process only, for v1).
- Not intended to replace production-grade auth (no bcrypt/Argon2, no
  rate limiting beyond per-account lockout, no HTTPS/TLS).
- No GUI.
- No concurrent-access support (single process, no file locking) in v1.

## 5. Target User

Primarily a portfolio/learning project: demonstrates C systems
programming skills (file I/O, structs, hashing, CLI design) for
technical interviews and coursework. Secondarily, a reusable base for a
future networked or role-based access control system.

## 6. Functional Requirements

| ID    | Requirement                                                                                |
| ----- | ------------------------------------------------------------------------------------------ |
| FR-1  | Users can register with a username, password, and role.                                    |
| FR-2  | Usernames must be unique; duplicate registration is rejected.                              |
| FR-3  | Passwords must be at least 4 characters (v1 minimum; see Open Questions).                  |
| FR-4  | Passwords are stored as`SHA-256(salt + password)`, never in plaintext.                   |
| FR-5  | Each user has a unique random salt generated at registration/password-change time.         |
| FR-6  | Users can log in with username + password.                                                 |
| FR-7  | After 3 consecutive failed login attempts, the account is locked for 30 seconds.           |
| FR-8  | The failed-attempt counter resets on successful login or after the lockout window expires. |
| FR-9  | Admins (via CLI, v1: any user) can list all active users.                                  |
| FR-10 | Admins can update a user's role.                                                           |
| FR-11 | Admins can change a user's password.                                                       |
| FR-12 | Admins can delete a user (soft delete — record is flagged inactive, not erased).          |
| FR-13 | Data persists across restarts via a binary file (`data/users.dat`).                      |
| FR-14 | Password input is not echoed to the terminal.                                              |

## 7. Non-Functional Requirements

- **Portability**: builds with `gcc -std=c11` on any POSIX system (uses `<termios.h>`, `<sys/stat.h>`).
- **No external dependencies**: SHA-256 implemented in-project, no OpenSSL/libcrypto.
- **Performance**: O(1) record access by id via fixed-size binary records.
- **Code quality**: compiles with `-Wall -Wextra` with zero warnings.

## 8. Success Metrics

Since this is a learning/portfolio project rather than a shipped
product, success is measured by:

- All functional requirements implemented and manually verified (see `docs/TESTING.md`).
- Clean compilation with no warnings.
- Code is modular enough that a reviewer (e.g., interviewer) can trace
  a request (e.g., login) through distinct, single-responsibility files.

## 9. Risks / Known Limitations

- **No file locking**: concurrent runs of the program could corrupt `users.dat`. Acceptable for v1 single-user CLI use; must be addressed before any multi-process or networked version.
- **SHA-256 without stretching**: a single SHA-256 pass is fast, which is good for legitimate logins but also good for offline brute-force if `users.dat` is ever exfiltrated. Production systems should use bcrypt/Argon2/PBKDF2 instead.
- **Fixed-size fields**: `username[32]`, `role[16]` truncate silently beyond their length rather than rejecting/erroring explicitly.
- **Per-account, not per-IP, lockout**: doesn't protect against distributed brute force (not applicable yet since there's no network layer).

## 10. Open Questions

- Should role changes/deletes be restricted to users with role `admin`, enforced at the CLI layer? (Currently any menu user can perform any action — see `docs/ARCHITECTURE.md` "Known Limitations".)
- Should the minimum password length/complexity be stricter than 4 characters?
- Is a networked (client-server) version in scope for a v2, or does this stay a local CLI tool?

## 11. Future Scope (v2 candidates)

- Role-based access control enforced at the CLI layer (see `README.md` "Extending it").
- TCP server wrapper for multi-client access.
- Swap binary file storage for SQLite.
- Upgrade hashing to bcrypt/Argon2.
