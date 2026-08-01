/**
 * @file function_info.h
 * @brief Registry of known function names and their required argument counts.
 *
 * This is the single source of truth the validator, parser, and
 * evaluator all consult to agree on which identifiers are functions
 * and how many arguments each one takes.
 */
#ifndef FUNCTION_INFO_H
#define FUNCTION_INFO_H

/** A function name paired with its required argument count. */
typedef struct
{
    const char *name; /**< Function name, e.g. "sin". */
    int argc;         /**< Required argument count (1 or 2). */
} FunctionInfo;

/** @brief Looks up how many arguments a function takes.
 *  @param name Function name.
 *  @return The argument count, or -1 if @p name isn't a recognized
 *  function. Callers should generally check isFunction() first
 *  rather than relying on the sentinel. */
int functionArgumentCount(const char name[]);

/** @brief Checks whether a name is a recognized function.
 *  @return 1 if @p name matches an entry in the registry, 0 otherwise. */
int isFunction(const char name[]);

#endif
