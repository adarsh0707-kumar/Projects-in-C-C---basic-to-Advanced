#ifndef STORAGE_H
#define STORAGE_H

#include <time.h>
#include "config.h"

typedef struct
{
    int id;
    char username[USERNAME_LEN];
    char salt[SALT_LEN];
    char password_hash[HASH_LEN]; /* sha256(salt + password) */
    char role[ROLE_LEN];          /* e.g. "admin", "user" */
    int failed_attempts;
    time_t lock_until; /* 0 = not locked */
    int active;        /* 0 = deleted (soft delete) */
} User;

/* Ensures data/ dir and the data file exist. Call once at startup. */
void storage_init(void);

/* Appends a new user record. Returns the new user's id, or -1 on failure. */
int storage_append(User *u);

/* Overwrites the record at the given id (must already exist). Returns 0 on success. */
int storage_update(User *u);

/* Loads the record with the given id into *out. Returns 0 on success, -1 if not found/inactive. */
int storage_find_by_id(int id, User *out);

/* Loads the record with the given username into *out. Returns 0 on success, -1 if not found. */
int storage_find_by_username(const char *username, User *out);

/* Marks a user inactive (soft delete). Returns 0 on success. */
int storage_delete(int id);

/* Calls cb(User*) for every active user. */
void storage_foreach(void (*cb)(const User *));

#endif /* STORAGE_H */
