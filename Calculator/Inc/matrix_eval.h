/**
 * @file matrix_eval.h
 * @brief C-facing entry point into the C++ matrix engine.
 */
#ifndef MATRIX_EVAL_H
#define MATRIX_EVAL_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Evaluates a matrix expression, e.g. "det([[1,2],[3,4]])",
     * "inverse([[1,2],[3,4]])", "transpose([[1,2],[3,4]])", or
     * "[[1,2],[3,4]] + [[5,6],[7,8]]".
     * @param expr       Null-terminated input expression.
     * @param result     Destination buffer for the formatted result on
     *                   success (a scalar like "5" for det(), or a
     *                   matrix like "[[6,8],[10,12]]"), or an error
     *                   message on failure.
     * @param resultSize Size of @p result in bytes.
     * @return 1 on success, 0 on failure. */
    int evaluateMatrixExpression(const char *expr, char *result, int resultSize);

#ifdef __cplusplus
}
#endif

#endif
