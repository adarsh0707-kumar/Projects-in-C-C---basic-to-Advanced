#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#include "storage.h"
#include "user.h"
#include "auth.h"
#include "log.h"

/* Reads a line of input without echoing it to the terminal (for passwords). */
static void read_hidden(char *buf, size_t buflen)
{
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (fgets(buf, (int)buflen, stdin) == NULL)
        buf[0] = '\0';
    buf[strcspn(buf, "\n")] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
}

static void read_line(char *buf, size_t buflen)
{
    if (fgets(buf, (int)buflen, stdin) == NULL)
    {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\n")] = '\0';
}

static void flush_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

static void do_register(void)
{
    char username[USERNAME_LEN], password[128], role[ROLE_LEN];

    printf("New username: ");
    read_line(username, sizeof(username));
    printf("Password (min 4 chars): ");
    read_hidden(password, sizeof(password));
    printf("Role [user/admin, default 'user']: ");
    read_line(role, sizeof(role));

    int result = user_register(username, password, role);
    if (result == -1)
    {
        printf("Registration failed: username already exists.\n");
    }
    else if (result == -2)
    {
        printf("Registration failed: invalid username or password too short.\n");
    }
    else
    {
        printf("User '%s' registered with id %d.\n", username, result);
    }
}

static void do_login(void)
{
    char username[USERNAME_LEN], password[128];
    printf("Username: ");
    read_line(username, sizeof(username));
    printf("Password: ");
    read_hidden(password, sizeof(password));

    int id;
    AuthResult r = auth_login(username, password, &id);
    switch (r)
    {
    case AUTH_OK:
        printf("Login successful. Welcome, %s (id %d).\n", username, id);
        break;
    case AUTH_BAD_CREDENTIALS:
        printf("Login failed: incorrect username or password.\n");
        break;
    case AUTH_LOCKED:
        printf("Account locked due to too many failed attempts. Try again shortly.\n");
        break;
    case AUTH_NOT_FOUND:
        printf("Login failed: no such user.\n");
        break;
    }
}

static void do_update_role(void)
{
    int id;
    char role[ROLE_LEN];
    printf("User id: ");
    if (scanf("%d", &id) != 1)
    {
        flush_stdin();
        printf("Invalid id.\n");
        return;
    }
    flush_stdin();
    printf("New role: ");
    read_line(role, sizeof(role));

    if (user_update_role(id, role) == 0)
        printf("Role updated.\n");
    else
        printf("User not found.\n");
}

static void do_change_password(void)
{
    int id;
    char password[128];
    printf("User id: ");
    if (scanf("%d", &id) != 1)
    {
        flush_stdin();
        printf("Invalid id.\n");
        return;
    }
    flush_stdin();
    printf("New password: ");
    read_hidden(password, sizeof(password));

    if (user_change_password(id, password) == 0)
        printf("Password updated.\n");
    else
        printf("User not found.\n");
}

static void do_delete(void)
{
    int id;
    printf("User id to delete: ");
    if (scanf("%d", &id) != 1)
    {
        flush_stdin();
        printf("Invalid id.\n");
        return;
    }
    flush_stdin();

    if (user_delete(id) == 0)
        printf("User %d deleted.\n", id);
    else
        printf("User not found.\n");
}

static void print_menu(void)
{
    printf("\n===== User Management System =====\n");
    printf("1. Register new user\n");
    printf("2. Login\n");
    printf("3. List all users\n");
    printf("4. Update user role\n");
    printf("5. Change password\n");
    printf("6. Delete user\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void)
{
    srand((unsigned int)time(NULL));
    storage_init();
    log_init();

    int choice;
    char line[16];

    while (1)
    {
        print_menu();
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        if (sscanf(line, "%d", &choice) != 1)
            continue;

        switch (choice)
        {
        case 1:
            do_register();
            break;
        case 2:
            do_login();
            break;
        case 3:
            user_list_all();
            break;
        case 4:
            do_update_role();
            break;
        case 5:
            do_change_password();
            break;
        case 6:
            do_delete();
            break;
        case 0:
            printf("Goodbye.\n");
            return 0;
        default:
            printf("Invalid choice.\n");
        }
    }
    return 0;
}
