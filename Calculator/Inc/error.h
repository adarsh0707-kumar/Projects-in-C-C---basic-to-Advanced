#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        CALC_OK = 0,

        CALC_ERR_UNKNOWN,
        CALC_ERR_DIVIDE_BY_ZERO,
        CALC_ERR_DOMAIN,
        CALC_ERR_OVERFLOW,
        CALC_ERR_UNDERFLOW,

        CALC_ERR_INVALID_TOKEN,
        CALC_ERR_INVALID_EXPRESSION,
        CALC_ERR_INVALID_FUNCTION,
        CALC_ERR_INVALID_VARIABLE,

        CALC_ERR_STACK_OVERFLOW,
        CALC_ERR_STACK_UNDERFLOW,

        CALC_ERR_MEMORY,
        CALC_ERR_FILE,
        CALC_ERR_INTERNAL

    } CalculatorError;

    typedef struct
    {
        CalculatorError code;
        const char *message;
    } CalculatorStatus;

    const CalculatorStatus *calculatorGetStatus(void);

    CalculatorError calculatorGetLastError(void);

    const char *calculatorErrorString(CalculatorError error);

    void calculatorSetLastError(CalculatorError error);

    void calculatorClearError(void);

#ifdef __cplusplus
}
#endif

#endif