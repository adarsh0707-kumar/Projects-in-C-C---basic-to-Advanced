#ifndef AUTH_H
#define AUTH_H

typedef enum
{
    AUTH_OK = 0,
    AUTH_BAD_CREDENTIALS,
    AUTH_LOCKED,
    AUTH_NOT_FOUND
} AuthResult;

/* Attempts to log in. On success, fills *out_id with the user's id.
   Updates failed_attempts / lock_until in storage as a side effect. */
AuthResult auth_login(const char *username, const char *password, int *out_id);

#endif /* AUTH_H */
