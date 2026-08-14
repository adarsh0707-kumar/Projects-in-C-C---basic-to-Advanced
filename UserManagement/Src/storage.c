#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "storage.h"

void storage_init(void)
{
    struct stat st = {0};
    if (stat("data", &st) == -1)
    {
        mkdir("data", 0700);
    }
    FILE *fp = fopen(DATA_FILE, "ab");
    if (!fp)
    {
        perror("storage_init: could not open data file");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
}

static long record_count(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    return size / (long)sizeof(User);
}

int storage_append(User *u)
{
    FILE *fp = fopen(DATA_FILE, "r+b");
    if (!fp)
        fp = fopen(DATA_FILE, "w+b");
    if (!fp)
        return -1;

    long count = record_count(fp);
    u->id = (int)count + 1;
    u->active = 1;

    fseek(fp, 0, SEEK_END);
    fwrite(u, sizeof(User), 1, fp);
    fclose(fp);
    return u->id;
}

int storage_update(User *u)
{
    FILE *fp = fopen(DATA_FILE, "r+b");
    if (!fp)
        return -1;

    long offset = (long)(u->id - 1) * (long)sizeof(User);
    fseek(fp, offset, SEEK_SET);
    size_t written = fwrite(u, sizeof(User), 1, fp);
    fclose(fp);
    return (written == 1) ? 0 : -1;
}

int storage_find_by_id(int id, User *out)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
        return -1;

    long offset = (long)(id - 1) * (long)sizeof(User);
    fseek(fp, offset, SEEK_SET);
    size_t read = fread(out, sizeof(User), 1, fp);
    fclose(fp);

    if (read != 1 || !out->active)
        return -1;
    return 0;
}

int storage_find_by_username(const char *username, User *out)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
        return -1;

    User u;
    while (fread(&u, sizeof(User), 1, fp) == 1)
    {
        if (u.active && strcmp(u.username, username) == 0)
        {
            *out = u;
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return -1;
}

int storage_delete(int id)
{
    User u;
    if (storage_find_by_id(id, &u) != 0)
        return -1;
    u.active = 0;
    return storage_update(&u);
}

void storage_foreach(void (*cb)(const User *))
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp)
        return;

    User u;
    while (fread(&u, sizeof(User), 1, fp) == 1)
    {
        if (u.active)
            cb(&u);
    }
    fclose(fp);
}
