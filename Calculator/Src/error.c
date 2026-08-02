#include "../Inc/error.h"

static CalculatorError lastError = CALC_OK;

const char *calculatorErrorString(CalculatorError error)
{
    switch (error)
    {
    case CALC_OK:
        return "No error";

    case CALC_ERR_DIVIDE_BY_ZERO:
        return "Division by zero";

    case CALC_ERR_DOMAIN:
        return "Value outside the valid domain";

    case CALC_ERR_INVALID_TOKEN:
        return "Invalid token";

    case CALC_ERR_INVALID_EXPRESSION:
        return "Invalid expression";

    case CALC_ERR_INVALID_FUNCTION:
        return "Unknown function";

    case CALC_ERR_INVALID_VARIABLE:
        return "Unknown variable";

    case CALC_ERR_STACK_OVERFLOW:
        return "Stack overflow";

    case CALC_ERR_STACK_UNDERFLOW:
        return "Stack underflow";

    case CALC_ERR_INTERNAL:
        return "Internal error";

    default:
        return "Unrecognized error code";
    }
}

CalculatorError calculatorGetLastError(void)
{
    return lastError;
}

void calculatorSetLastError(CalculatorError error)
{
    lastError = error;
}

void calculatorClearError(void)
{
    lastError = CALC_OK;
}
