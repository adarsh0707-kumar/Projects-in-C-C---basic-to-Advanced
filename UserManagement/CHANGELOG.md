# Changelog

All notable changes to this project are documented here.
Format loosely follows [Keep a Changelog](https://keepachangelog.com/).

## [1.0.0] — 2026-08-14

### Added

- Initial release: CLI-based user management system in C.
- Registration, login, list, update role, change password, delete (soft delete).
- SHA-256 implemented from scratch (`src/sha256.c`), no external crypto dependency.
- Per-user random salt; passwords stored as `sha256(salt + password)`.
- Login lockout: 3 failed attempts → 30s lockout, auto-reset on expiry or success.
- Binary file storage (`data/users.dat`) with O(1) record access by id.
- Hidden password input via `termios`.
- `docs/PRD.md`, `docs/ARCHITECTURE.md`, `docs/TESTING.md`.

### Known limitations (see `docs/PRD.md` §9 for detail)

- No authorization/session layer — any menu user can update/delete any account.
- No file locking — not safe for concurrent multi-process use.
- Single-pass SHA-256, not an adaptive hash (bcrypt/Argon2) — acceptable for
  a learning project, not for production credential storage.

## [Unreleased]

Candidates for the next version — see `docs/PRD.md` §11 "Future Scope":

- Role-based access control enforced at the CLI layer.
- TCP server wrapper for multi-client access.
- SQLite storage backend option.
- Automated test harness (see `docs/TESTING.md` "Future: automated tests").
