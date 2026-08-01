#include "../Inc/error.h"

static CalculatorStatus status =
    {
        CALC_OK,
        "No error"};

const CalculatorStatus *calculatorGetStatus(void)
{
    return &status;
}

CalculatorError calculatorGetLastError(void)
{
    return status.code;
}

const char *calculatorErrorString(CalculatorError error)
{
    switch (error)
    {
    case CALC_OK:
        return "No error";

    case CALC_ERR_UNKNOWN:
        return "Unknown error";

    case CALC_ERR_DIVIDE_BY_ZERO:
        return "Division by zero";

    case CALC_ERR_DOMAIN:
        return "Domain error";

    case CALC_ERR_OVERFLOW:
        return "Overflow";

    case CALC_ERR_UNDERFLOW:
        return "Underflow";

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

    case CALC_ERR_MEMORY:
        return "Memory allocation failed";

    case CALC_ERR_FILE:
        return "File error";

    case CALC_ERR_INTERNAL:
        return "Internal error";

    default:
        return "Unknown error";
    }
}

void calculatorSetLastError(CalculatorError error)
{
    status.code = error;
    status.message = calculatorErrorString(error);
}

void calculatorClearError(void)
{
    status.code = CALC_OK;
    status.message = "No error";
}