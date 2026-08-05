/**
 * @file variables.h
 * @brief User-defined variables, plus the special "ans" (last result).
 */
#ifndef VARIABLES_H
#define VARIABLES_H

/** Maximum number of user-defined variables that can be stored at once. */
#define MAX_VARIABLES 100

/** A single named variable/value pair. */
typedef struct
{
    char name[32]; /**< Variable name, e.g. "x". */
    double value;  /**< Current value. */
} Variable;

/** @brief Sets the value of the implicit "ans" variable (the result of
 *  the last successful calculation). */
void setAns(double value);
/** @brief Returns the current value of "ans". */
double getAns(void);

/** @brief Creates or updates a variable.
 *  @param name  Variable name. Rejected if it collides with a built-in
 *  constant ("pi", "e") -- see constants.h.
 *  @param value New value.
 *  @return 1 on success, 0 if the name is reserved or the variable
 *  table is full. */
int setVariable(const char name[], double value);

/** @brief Looks up a variable's value.
 *  @param name  Variable name to look up.
 *  @param value Out-parameter set to the variable's value on success.
 *  @return 1 if found, 0 otherwise. */
int getVariable(const char name[], double *value);

/** @brief Prints every currently-defined variable to stdout. */
void showVariables(void);

/** @brief Returns the number of variables currently defined (always
 *  includes the built-ins "ans", "pi", "e"). */
int getVariableCount(void);

/** @brief Writes the name and value of variable #index (0-indexed,
 *  same order as showVariables()) into outName/outValue.
 *  @param index    0-indexed position.
 *  @param outName  Destination buffer for the name.
 *  @param outSize  Size of outName in bytes.
 *  @param outValue Out-parameter set to the variable's value.
 *  @return 1 on success, 0 if index is out of range. */
int getVariableByIndex(int index, char outName[], int outSize, double *outValue);

#endif
