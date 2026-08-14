#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include "log.h"
#include "config.h"

void log_init(void)
{
    struct stat st = {0};
    if (stat("log", &st) == -1)
    {
        mkdir("log", 0700);
    }
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp)
        fclose(fp);
}

static const char *level_str(LogLevel level)
{
    switch (level)
    {
    case LOG_INFO:
        return "INFO ";
    case LOG_WARN:
        return "WARN ";
    case LOG_ERROR:
        return "ERROR";
    default:
        return "?????";
    }
}

void log_write(LogLevel level, const char *fmt, ...)
{
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp)
        return;

    time_t now = time(NULL);
    char timebuf[20];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(fp, "[%s] [%s] ", timebuf, level_str(level));

    va_list args;
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);

    fprintf(fp, "\n");
    fclose(fp);
}
