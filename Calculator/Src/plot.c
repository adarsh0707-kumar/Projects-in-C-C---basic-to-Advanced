#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "plot.h"
#include "calculator.h"
#include "validator.h"
#include "variables.h"

#define PLOT_WIDTH 61  /* columns -- odd, so there's a true centre column */
#define PLOT_HEIGHT 21 /* rows    -- odd, so there's a true centre row    */
#define PLOT_XMIN -10.0
#define PLOT_XMAX 10.0

/* Trims leading/trailing whitespace in place. */
static char *trim(char *s)
{
    while (isspace((unsigned char)*s))
        s++;

    if (*s == '\0')
        return s;

    char *end = s + strlen(s) - 1;

    while (end > s && isspace((unsigned char)*end))
    {
        *end = '\0';
        end--;
    }

    return s;
}

/* Extracts the expression inside "plot( ... )", tracking paren depth
   so that nested calls like plot(sin(x)) are handled correctly.
   Returns 1 on success and fills innerExpr; 0 on a syntax problem
   (and fills errorMsg). */
static int extractPlotArgument(const char *trimmedInput,
                               char innerExpr[],
                               size_t innerSize,
                               char errorMsg[],
                               size_t errorSize)
{
    const char prefix[] = "plot(";
    size_t prefixLen = strlen(prefix);

    if (strncmp(trimmedInput, prefix, prefixLen) != 0)
    {
        snprintf(errorMsg, errorSize, "Error: Use 'plot(<expression in x>)'.");
        return 0;
    }

    size_t i = prefixLen;
    int depth = 1; /* the '(' in "plot(" already opened one level */
    size_t start = i;
    size_t closeAt = 0;
    int found = 0;

    for (; trimmedInput[i] != '\0'; i++)
    {
        if (trimmedInput[i] == '(')
            depth++;
        else if (trimmedInput[i] == ')')
        {
            depth--;

            if (depth == 0)
            {
                closeAt = i;
                found = 1;
                break;
            }
        }
    }

    if (!found)
    {
        snprintf(errorMsg, errorSize, "Error: Missing closing ')' in plot(...).");
        return 0;
    }

    if (*trim((char *)trimmedInput + closeAt + 1) != '\0')
    {
        snprintf(errorMsg, errorSize, "Error: Unexpected characters after plot(...).");
        return 0;
    }

    size_t len = closeAt - start;

    if (len == 0)
    {
        snprintf(errorMsg, errorSize, "Error: plot() requires an expression, e.g. plot(sin(x)).");
        return 0;
    }

    if (len >= innerSize)
        len = innerSize - 1;

    strncpy(innerExpr, trimmedInput + start, len);
    innerExpr[len] = '\0';

    char *trimmedInner = trim(innerExpr);

    if (trimmedInner != innerExpr)
        memmove(innerExpr, trimmedInner, strlen(trimmedInner) + 1);

    return 1;
}

int evaluatePlotExpression(const char input[], char errorMsg[], size_t errorSize)
{
    errorMsg[0] = '\0';

    char copy[256];
    strncpy(copy, input, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char *trimmedInput = trim(copy);
    char innerExpr[200];

    if (!extractPlotArgument(trimmedInput, innerExpr, sizeof(innerExpr), errorMsg, errorSize))
        return 0;

    char processed[400];
    insertImplicitMultiplication(innerExpr, processed);

    /* Syntax only needs checking once -- it doesn't depend on x's value. */
    if (!validateExpression(processed))
        return 0; /* validateExpression() already printed its own message */

    if (!validateParentheses(processed))
    {
        snprintf(errorMsg, errorSize, "Error: Mismatched parentheses in plot(...).");
        return 0;
    }

    double ys[PLOT_WIDTH];
    int valid[PLOT_WIDTH];
    double ymin = INFINITY;
    double ymax = -INFINITY;
    int anyValid = 0;

    for (int col = 0; col < PLOT_WIDTH; col++)
    {
        double x = PLOT_XMIN + col * (PLOT_XMAX - PLOT_XMIN) / (PLOT_WIDTH - 1);

        setVariable("x", x);

        char postfix[400];
        infixToPostfix(processed, postfix);

        double y = evaluatePostfix(postfix);

        if (isfinite(y))
        {
            ys[col] = y;
            valid[col] = 1;
            anyValid = 1;

            if (y < ymin)
                ymin = y;
            if (y > ymax)
                ymax = y;
        }
        else
        {
            valid[col] = 0;
        }
    }

    if (!anyValid)
    {
        snprintf(errorMsg, errorSize,
                 "Error: '%s' produced no finite values for x in [%.0f, %.0f].",
                 innerExpr, PLOT_XMIN, PLOT_XMAX);
        return 0;
    }

    /* Flat line (or a single valid point) -- pad the range so scaling
       below doesn't divide by zero. */
    if (ymax - ymin < 1e-9)
    {
        ymin -= 1.0;
        ymax += 1.0;
    }

    char canvas[PLOT_HEIGHT][PLOT_WIDTH + 1];

    for (int r = 0; r < PLOT_HEIGHT; r++)
    {
        memset(canvas[r], ' ', PLOT_WIDTH);
        canvas[r][PLOT_WIDTH] = '\0';
    }

    /* x-axis (y = 0), drawn first so data points can overwrite it */
    if (0.0 >= ymin && 0.0 <= ymax)
    {
        int zeroRow = (int)((0.0 - ymin) / (ymax - ymin) * (PLOT_HEIGHT - 1) + 0.5);
        int displayZeroRow = PLOT_HEIGHT - 1 - zeroRow;

        for (int col = 0; col < PLOT_WIDTH; col++)
            canvas[displayZeroRow][col] = '-';
    }

    /* y-axis (x = 0) */
    if (0.0 >= PLOT_XMIN && 0.0 <= PLOT_XMAX)
    {
        int zeroCol = (int)((0.0 - PLOT_XMIN) / (PLOT_XMAX - PLOT_XMIN) * (PLOT_WIDTH - 1) + 0.5);

        for (int r = 0; r < PLOT_HEIGHT; r++)
        {
            if (canvas[r][zeroCol] == '-')
                canvas[r][zeroCol] = '+';
            else if (canvas[r][zeroCol] == ' ')
                canvas[r][zeroCol] = '|';
        }
    }

    for (int col = 0; col < PLOT_WIDTH; col++)
    {
        if (!valid[col])
            continue;

        int row = (int)((ys[col] - ymin) / (ymax - ymin) * (PLOT_HEIGHT - 1) + 0.5);
        int displayRow = PLOT_HEIGHT - 1 - row;

        if (displayRow < 0)
            displayRow = 0;
        if (displayRow >= PLOT_HEIGHT)
            displayRow = PLOT_HEIGHT - 1;

        canvas[displayRow][col] = '*';
    }

    printf("\nPlot of %s for x in [%.1f, %.1f]\n\n", innerExpr, PLOT_XMIN, PLOT_XMAX);

    for (int r = 0; r < PLOT_HEIGHT; r++)
    {
        double label = ymax - r * (ymax - ymin) / (PLOT_HEIGHT - 1);
        printf("%9.2f |%s\n", label, canvas[r]);
    }

    printf("%10s", "");

    for (int c = 0; c < 10; c++)
        printf("-");
    printf("+");

    for (int c = 0; c < PLOT_WIDTH - 10; c++)
        printf("-");

    printf("\n%10.1f%*s0%*s%.1f\n",
           PLOT_XMIN,
           PLOT_WIDTH / 2 - 1, "",
           PLOT_WIDTH / 2 - 2, "",
           PLOT_XMAX);

    return 1;
}