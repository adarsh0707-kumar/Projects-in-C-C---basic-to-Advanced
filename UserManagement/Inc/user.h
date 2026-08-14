#ifndef USER_H
#define USER_H

#include "storage.h"

/* Registers a new user with a hashed+salted password.
   Returns new user id on success, -1 on duplicate username, -2 on invalid input. */
int user_register(const char *username, const char *password, const char *role);

/* Updates an existing user's role. Returns 0 on success, -1 if not found. */
int user_update_role(int id, const char *new_role);

/* Changes a user's password (re-salts and re-hashes). Returns 0 on success, -1 if not found. */
int user_change_password(int id, const char *new_password);

/* Soft-deletes a user. Returns 0 on success, -1 if not found. */
int user_delete(int id);

/* Prints all active users in a table to stdout. */
void user_list_all(void);

#endif /* USER_H */
