#ifndef COMPLEX_EVAL_H
#define COMPLEX_EVAL_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Evaluates a complex-number expression, e.g. "(2+3i)*(4-5i)" or
     * "sqrt(-1)", and writes the formatted result (e.g. "23+2i") into
     * result[resultSize]. Returns 1 on success. On failure, returns 0
     * and writes an error message into result instead.
     */
    int evaluateComplexExpression(const char *expr, char *result, int resultSize);

#ifdef __cplusplus
}
#endif

#endif