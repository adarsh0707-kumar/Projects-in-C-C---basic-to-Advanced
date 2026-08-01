/**
 * @file complex_eval.h
 * @brief C-facing entry point into the C++ complex-number engine.
 */
#ifndef COMPLEX_EVAL_H
#define COMPLEX_EVAL_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Evaluates a complex-number expression, e.g. "(2+3i)*(4-5i)"
     * or "sqrt(-1)".
     * @param expr       Null-terminated input expression.
     * @param result     Destination buffer for the formatted result
     *                   (e.g. "23+2i") on success, or an error message
     *                   on failure.
     * @param resultSize Size of @p result in bytes.
     * @return 1 on success, 0 on failure. */
    int evaluateComplexExpression(const char *expr, char *result, int resultSize);

#ifdef __cplusplus
}
#endif

#endif
