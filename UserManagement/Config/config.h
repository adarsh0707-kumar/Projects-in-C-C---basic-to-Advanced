#ifndef CONFIG_H
#define CONFIG_H

/*
 * Central place for tunable constants. Keeping these out of storage.h
 * / auth.h means changing a policy (e.g. lockout duration) doesn't
 * require touching the modules that implement it.
 */

/* Field sizes (bytes, including null terminator) */
#define USERNAME_LEN 32
#define ROLE_LEN 16
#define SALT_LEN 17 /* 16 hex chars + null */
#define HASH_LEN 65 /* 64 hex chars + null */

/* Password policy */
#define MIN_PASSWORD_LEN 4

/* Login lockout policy */
#define MAX_FAILED_ATTEMPTS 3
#define LOCKOUT_SECONDS 30

/* File paths */
#define DATA_FILE "data/users.dat"
#define LOG_FILE "log/app.log"

#endif /* CONFIG_H */
