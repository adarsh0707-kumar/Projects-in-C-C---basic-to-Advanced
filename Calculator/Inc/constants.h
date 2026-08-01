/**
 * @file constants.h
 * @brief Built-in mathematical constants ("pi", "e").
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

/** @brief Returns the value of a built-in constant.
 *  @param name "pi" or "e".
 *  @return The constant's value, or 0 if @p name isn't recognized --
 *  callers should check isConstant() first. */
double getConstant(const char name[]);

/** @brief Checks whether a name refers to a built-in constant.
 *  @return 1 if @p name is "pi" or "e", 0 otherwise. */
int isConstant(const char name[]);

#endif
