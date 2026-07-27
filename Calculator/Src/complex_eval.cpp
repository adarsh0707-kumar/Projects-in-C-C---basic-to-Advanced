#include "complex_eval.h"
#include "complex.hpp"
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace
{

    /*
     * Small recursive-descent parser, scoped only to what complex-number
     * expressions actually need: numbers (real or imaginary via an 'i'
     * suffix), + - * /, parentheses, unary +/-, and sqrt(). This is
     * intentionally much simpler than the main calculator's grammar —
     * no variables, no multi-arg functions, no factorial — because it
     * only has to serve this one isolated feature.
     *
     * Grammar:
     *   expr   := term (('+' | '-') term)*
     *   term   := factor (('*' | '/') factor)*
     *   factor := '(' expr ')' | 'sqrt' '(' expr ')' | ('+' | '-') factor
     *           | number ['i'] | 'i'
     */
    class ComplexParser
    {
    public:
        explicit ComplexParser(const std::string &s) : src(s), pos(0) {}

        Complex parse()
        {
            Complex result = parseExpr();
            skipSpaces();

            if (pos != src.size())
            {
                throw std::runtime_error(
                    "Unexpected character '" + std::string(1, src[pos]) + "'");
            }

            return result;
        }

    private:
        const std::string &src;
        size_t pos;

        void skipSpaces()
        {
            while (pos < src.size() && std::isspace((unsigned char)src[pos]))
                pos++;
        }

        char peek()
        {
            skipSpaces();
            return (pos < src.size()) ? src[pos] : '\0';
        }

        Complex parseExpr()
        {
            Complex value = parseTerm();

            while (true)
            {
                char c = peek();

                if (c == '+')
                {
                    pos++;
                    value = value + parseTerm();
                }
                else if (c == '-')
                {
                    pos++;
                    value = value - parseTerm();
                }
                else
                {
                    break;
                }
            }

            return value;
        }

        Complex parseTerm()
        {
            Complex value = parseFactor();

            while (true)
            {
                char c = peek();

                if (c == '*')
                {
                    pos++;
                    value = value * parseFactor();
                }
                else if (c == '/')
                {
                    pos++;
                    value = value / parseFactor();
                }
                else
                {
                    break;
                }
            }

            return value;
        }

        Complex parseFactor()
        {
            char c = peek();

            if (c == '-')
            {
                pos++;
                return -parseFactor();
            }

            if (c == '+')
            {
                pos++;
                return parseFactor();
            }

            if (c == '(')
            {
                pos++;
                Complex value = parseExpr();

                if (peek() != ')')
                    throw std::runtime_error("Mismatched parentheses");

                pos++;
                return value;
            }

            if (std::isalpha((unsigned char)c))
            {
                std::string name;

                while (pos < src.size() && std::isalpha((unsigned char)src[pos]))
                    name += src[pos++];

                if (name == "sqrt")
                {
                    if (peek() != '(')
                        throw std::runtime_error("Expected '(' after sqrt");

                    pos++;
                    Complex arg = parseExpr();

                    if (peek() != ')')
                        throw std::runtime_error("Mismatched parentheses");

                    pos++;
                    return Complex::sqrtOf(arg);
                }

                if (name == "i")
                    return Complex(0.0, 1.0);

                throw std::runtime_error("Unknown identifier '" + name + "'");
            }

            if (std::isdigit((unsigned char)c) || c == '.')
            {
                size_t start = pos;
                int dotCount = 0;

                while (pos < src.size() &&
                       (std::isdigit((unsigned char)src[pos]) || src[pos] == '.'))
                {
                    if (src[pos] == '.')
                        dotCount++;

                    if (dotCount > 1)
                        throw std::runtime_error("Invalid number");

                    pos++;
                }

                double num = std::atof(src.substr(start, pos - start).c_str());

                skipSpaces();

                if (pos < src.size() && src[pos] == 'i')
                {
                    pos++;
                    return Complex(0.0, num);
                }

                return Complex(num, 0.0);
            }

            throw std::runtime_error("Operand expected");
        }
    };

} /* anonymous namespace */

int evaluateComplexExpression(const char *expr, char *result, int resultSize)
{
    try
    {
        std::string input(expr);
        ComplexParser parser(input);
        Complex value = parser.parse();
        std::string text = value.toString();

        std::strncpy(result, text.c_str(), resultSize - 1);
        result[resultSize - 1] = '\0';
        return 1;
    }
    catch (const std::exception &e)
    {
        std::strncpy(result, e.what(), resultSize - 1);
        result[resultSize - 1] = '\0';
        return 0;
    }
}