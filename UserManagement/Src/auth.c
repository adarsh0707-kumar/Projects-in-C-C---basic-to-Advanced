#include <stdio.h>
#include <string.h>
#include <time.h>
#include "auth.h"
#include "storage.h"
#include "sha256.h"
#include "log.h"

static void hash_password(const char *salt, const char *password, char out_hash[HASH_LEN])
{
    char combined[SALT_LEN + 128];
    snprintf(combined, sizeof(combined), "%s%s", salt, password);
    sha256_hex(combined, out_hash);
}

AuthResult auth_login(const char *username, const char *password, int *out_id)
{
    User u;
    if (storage_find_by_username(username, &u) != 0)
        return AUTH_NOT_FOUND;

    time_t now = time(NULL);

    /* Still within a lockout window? */
    if (u.lock_until > now)
    {
        log_write(LOG_WARN, "Login blocked (locked): user '%s'", username);
        return AUTH_LOCKED;
    }

    /* Lockout window expired: reset the counter before trying again. */
    if (u.lock_until != 0 && u.lock_until <= now)
    {
        u.failed_attempts = 0;
        u.lock_until = 0;
    }

    char computed_hash[HASH_LEN];
    hash_password(u.salt, password, computed_hash);

    if (strcmp(computed_hash, u.password_hash) == 0)
    {
        u.failed_attempts = 0;
        u.lock_until = 0;
        storage_update(&u);
        log_write(LOG_INFO, "Login success: user '%s' (id %d)", username, u.id);
        *out_id = u.id;
        return AUTH_OK;
    }

    /* Wrong password: increment counter, lock if threshold hit. */
    u.failed_attempts++;
    if (u.failed_attempts >= MAX_FAILED_ATTEMPTS)
    {
        u.lock_until = now + LOCKOUT_SECONDS;
        log_write(LOG_WARN, "User '%s' locked out after %d failed attempts", username, u.failed_attempts);
    }
    else
    {
        log_write(LOG_WARN, "Failed login attempt %d for user '%s'", u.failed_attempts, username);
    }
    storage_update(&u);

    return (u.lock_until > now) ? AUTH_LOCKED : AUTH_BAD_CREDENTIALS;
}
