/**
 * @file error.h
 * @brief Centralized, recoverable error reporting for the calculator engine.
 *
 * Historically, unrecoverable-looking conditions (stack overflow,
 * stack underflow, division by zero, ...) were handled by printing a
 * message and calling exit(), which works fine for a single top-level
 * calculation but is fatal for any caller that evaluates many
 * expressions in a loop -- see the plot(1/x)/plot(x!) crash fixed in
 * docs/CHANGELOG.md.
 *
 * This module gives modules a way to report *why* an operation failed
 * without killing the process, following docs/RULES.md's own
 * guidance: "Never call exit() from library modules; return status
 * codes instead." It's being adopted incrementally, module by module
 * (see docs/CHANGELOG.md for progress) rather than all at once, so
 * the codebase stays buildable and fully tested at every step.
 */
#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif

    /** Every recoverable error condition the engine can report. */
    typedef enum
    {
        CALC_OK = 0,                 /**< No error. */
        CALC_ERR_DIVIDE_BY_ZERO,     /**< Division or modulus by zero. */
        CALC_ERR_DOMAIN,             /**< Argument outside a function's valid domain. */
        CALC_ERR_INVALID_TOKEN,      /**< Malformed token while parsing. */
        CALC_ERR_INVALID_EXPRESSION, /**< Expression failed syntax validation. */
        CALC_ERR_INVALID_FUNCTION,   /**< Unrecognized function name. */
        CALC_ERR_INVALID_VARIABLE,   /**< Unrecognized variable name. */
        CALC_ERR_STACK_OVERFLOW,     /**< Too many elements pushed onto a fixed-size stack. */
        CALC_ERR_STACK_UNDERFLOW,    /**< Pop/peek attempted on an empty stack. */
        CALC_ERR_INTERNAL            /**< Anything else unexpected. */
    } CalculatorError;

    /** @brief Returns a short, human-readable description of @p error. */
    const char *calculatorErrorString(CalculatorError error);

    /** @brief Returns the most recently set error (CALC_OK if none, or
     *  if calculatorClearError() was called since). */
    CalculatorError calculatorGetLastError(void);

    /** @brief Records that @p error just occurred. Overwrites any
     *  previously recorded error. */
    void calculatorSetLastError(CalculatorError error);

    /** @brief Resets the last-error state back to CALC_OK. */
    void calculatorClearError(void);

#ifdef __cplusplus
}
#endif

#endif
