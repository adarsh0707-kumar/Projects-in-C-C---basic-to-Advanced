#ifndef MATRIX_EVAL_H
#define MATRIX_EVAL_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Evaluates a matrix expression, e.g. "det([[1,2],[3,4]])",
     * "inverse([[1,2],[3,4]])", "transpose([[1,2],[3,4]])", or
     * "[[1,2],[3,4]] + [[5,6],[7,8]]", and writes the formatted result
     * into result[resultSize]. A scalar result (e.g. from det()) prints
     * as a plain number; a matrix result prints as "[[...],[...]]".
     * Returns 1 on success. On failure, returns 0 and writes an error
     * message into result instead.
     */
    int evaluateMatrixExpression(const char *expr, char *result, int resultSize);

#ifdef __cplusplus
}
#endif

#endif