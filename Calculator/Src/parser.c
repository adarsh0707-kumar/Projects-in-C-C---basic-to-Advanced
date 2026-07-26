#include <ctype.h>
#include <string.h>
#include "stack.h"
#include "calculator.h"
#include "functions.h"

/*
 * Walks backward from position i over a contiguous run of
 * alnum/'_' characters. If that run contains a letter, the
 * character at i is part of an identifier (e.g. the '2' in
 * "atan2"), not a standalone numeric literal.
 */
static int isPartOfIdentifier(char input[], int i)
{
    while (i >= 0 && (isalnum((unsigned char)input[i]) || input[i] == '_'))
    {
        if (isalpha((unsigned char)input[i]))
            return 1;
        i--;
    }
    return 0;
}

int validateParentheses(char expression[])
{
    CharStack s;
    initCharStack(&s);

    int i = 0;

    while (expression[i] != '\0')
    {
        if (expression[i] == '(')
        {
            pushChar(&s, '(');
        }
        else if (expression[i] == ')')
        {
            if (isEmptyCharStack(&s))
            {
                return 0;
            }
            popChar(&s);
        }
        i++;
    }
    return isEmptyCharStack(&s);
}

void insertImplicitMultiplication(char input[], char output[])
{
    int i = 0;
    int j = 0;

    while (input[i] != '\0')
    {
        /* Copy current character */
        output[j++] = input[i];

        if (input[i + 1] == '\0')
        {
            i++;
            continue;
        }

        char current = input[i];
        char next = input[i + 1];

        /* Skip spaces */
        if (isspace(current) || isspace(next))
        {
            i++;
            continue;
        }

        int insert = 0;

        /* -------------------------
           Number followed by '('
           or identifier
        --------------------------*/
        if ((isdigit(current) || current == '.') &&
            (next == '(' || isalpha(next)) &&
            !isPartOfIdentifier(input, i))
        {
            insert = 1;
        }

        /* -------------------------
           ')' followed by number,
           identifier or '('
        --------------------------*/
        else if (current == ')' &&
                 (isdigit(next) ||
                  isalpha(next) ||
                  next == '('))
        {
            insert = 1;
        }

        /*
         * IMPORTANT:
         * Never insert '*' inside an identifier.
         * Never insert '*' between a function name
         * and '('.
         */

        if (insert)
            output[j++] = '*';

        i++;
    }

    output[j] = '\0';
}