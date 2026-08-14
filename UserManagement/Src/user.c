#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "user.h"
#include "sha256.h"
#include "log.h"

/* Generates a random 16-hex-char salt. Seeded once by caller (main). */
static void generate_salt(char out[SALT_LEN])
{
    static const char hex_chars[] = "0123456789abcdef";
    for (int i = 0; i < SALT_LEN - 1; ++i)
        out[i] = hex_chars[rand() % 16];
    out[SALT_LEN - 1] = '\0';
}

static void hash_password(const char *salt, const char *password, char out_hash[HASH_LEN])
{
    char combined[SALT_LEN + 128];
    snprintf(combined, sizeof(combined), "%s%s", salt, password);
    sha256_hex(combined, out_hash);
}

int user_register(const char *username, const char *password, const char *role)
{
    if (!username || !password || strlen(username) == 0 || strlen(password) < MIN_PASSWORD_LEN)
        return -2;
    if (strlen(username) >= USERNAME_LEN)
        return -2;

    User existing;
    if (storage_find_by_username(username, &existing) == 0)
    {
        log_write(LOG_WARN, "Registration rejected: duplicate username '%s'", username);
        return -1; /* duplicate */
    }

    User u = {0};
    strncpy(u.username, username, USERNAME_LEN - 1);
    strncpy(u.role, role && strlen(role) ? role : "user", ROLE_LEN - 1);
    generate_salt(u.salt);
    hash_password(u.salt, password, u.password_hash);
    u.failed_attempts = 0;
    u.lock_until = 0;

    int id = storage_append(&u);
    if (id > 0)
        log_write(LOG_INFO, "User '%s' registered with id %d", username, id);
    return id;
}

int user_update_role(int id, const char *new_role)
{
    User u;
    if (storage_find_by_id(id, &u) != 0)
        return -1;
    strncpy(u.role, new_role, ROLE_LEN - 1);
    u.role[ROLE_LEN - 1] = '\0';
    int rc = storage_update(&u);
    if (rc == 0)
        log_write(LOG_INFO, "User id %d role changed to '%s'", id, new_role);
    return rc;
}

int user_change_password(int id, const char *new_password)
{
    User u;
    if (storage_find_by_id(id, &u) != 0)
        return -1;
    generate_salt(u.salt);
    hash_password(u.salt, new_password, u.password_hash);
    u.failed_attempts = 0;
    u.lock_until = 0;
    int rc = storage_update(&u);
    if (rc == 0)
        log_write(LOG_INFO, "User id %d changed password", id);
    return rc;
}

int user_delete(int id)
{
    int rc = storage_delete(id);
    if (rc == 0)
        log_write(LOG_INFO, "User id %d deleted", id);
    return rc;
}

static void print_user_row(const User *u)
{
    printf("%-4d %-20s %-10s %-10s\n",
           u->id, u->username, u->role,
           u->lock_until > time(NULL) ? "LOCKED" : "active");
}

void user_list_all(void)
{
    printf("%-4s %-20s %-10s %-10s\n", "ID", "Username", "Role", "Status");
    printf("--------------------------------------------------\n");
    storage_foreach(print_user_row);
}
