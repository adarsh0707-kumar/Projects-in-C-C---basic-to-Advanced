#ifndef LOG_H
#define LOG_H

typedef enum
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

/* Ensures log/ dir exists and the log file is writable. Call once at startup. */
void log_init(void);

/* Appends a timestamped, leveled line to LOG_FILE. printf-style formatting. */
void log_write(LogLevel level, const char *fmt, ...);

#endif /* LOG_H */
