#include "matrix_eval.h"
#include "Matrix.hpp"
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <string>

namespace
{

    /*
     * A parsed value is either a plain scalar (the result of det(), or
     * a bare number) or a Matrix. Both can appear in the same
     * expression (e.g. "2*[[1,2],[3,4]]" or "det(A)+3"), so the parser
     * carries this small tagged union between grammar rules rather than
     * committing to one C++ type throughout.
     */
    struct Value
    {
        bool isScalar;
        double scalar;
        Matrix matrix;

        explicit Value(double s) : isScalar(true), scalar(s), matrix() {}
        explicit Value(const Matrix &m) : isScalar(false), scalar(0.0), matrix(m) {}

        std::string toString() const
        {
            if (isScalar)
            {
                std::ostringstream oss;
                oss << scalar;
                return oss.str();
            }
            return matrix.toString();
        }
    };

    Value valueAdd(const Value &a, const Value &b)
    {
        if (a.isScalar && b.isScalar)
            return Value(a.scalar + b.scalar);
        if (!a.isScalar && !b.isScalar)
            return Value(a.matrix + b.matrix);
        throw std::runtime_error("Cannot add a scalar and a matrix");
    }

    Value valueSub(const Value &a, const Value &b)
    {
        if (a.isScalar && b.isScalar)
            return Value(a.scalar - b.scalar);
        if (!a.isScalar && !b.isScalar)
            return Value(a.matrix - b.matrix);
        throw std::runtime_error("Cannot subtract a scalar and a matrix");
    }

    Value valueMul(const Value &a, const Value &b)
    {
        if (a.isScalar && b.isScalar)
            return Value(a.scalar * b.scalar);
        if (a.isScalar && !b.isScalar)
            return Value(b.matrix * a.scalar);
        if (!a.isScalar && b.isScalar)
            return Value(a.matrix * b.scalar);
        return Value(a.matrix * b.matrix); /* throws on dimension mismatch */
    }

    Value valueNegate(const Value &a)
    {
        if (a.isScalar)
            return Value(-a.scalar);
        return Value(a.matrix * -1.0);
    }

    /*
     * Grammar:
     *   expr    := term (('+' | '-') term)*
     *   term    := factor ('*' factor)*
     *   factor  := '(' expr ')' | ('det'|'inverse'|'transpose') '(' expr ')'
     *            | matrixLiteral | number | ('+' | '-') factor
     *   matrixLiteral := '[' row (',' row)* ']'
     *   row     := '[' number (',' number)* ']'
     */
    class MatrixParser
    {
    public:
        explicit MatrixParser(const std::string &s) : src(s), pos(0) {}

        Value parse()
        {
            Value result = parseExpr();
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

        double parseNumber()
        {
            skipSpaces();
            size_t start = pos;

            if (pos < src.size() && (src[pos] == '-' || src[pos] == '+'))
                pos++;

            int dotCount = 0;
            bool hasDigits = false;

            while (pos < src.size() &&
                   (std::isdigit((unsigned char)src[pos]) || src[pos] == '.'))
            {
                if (src[pos] == '.')
                    dotCount++;
                else
                    hasDigits = true;

                if (dotCount > 1)
                    throw std::runtime_error("Invalid number");

                pos++;
            }

            if (!hasDigits)
                throw std::runtime_error("Expected a number");

            return std::atof(src.substr(start, pos - start).c_str());
        }

        Matrix parseMatrixLiteral()
        {
            pos++; /* consume outer '[' */

            std::vector<std::vector<double>> rows;

            while (true)
            {
                if (peek() != '[')
                    throw std::runtime_error("Expected '[' to start a matrix row");
                pos++;

                std::vector<double> row;
                while (true)
                {
                    row.push_back(parseNumber());

                    if (peek() == ',')
                    {
                        pos++;
                        continue;
                    }
                    break;
                }

                if (peek() != ']')
                    throw std::runtime_error("Expected ']' to close a matrix row");
                pos++;

                rows.push_back(row);

                if (peek() == ',')
                {
                    pos++;
                    continue;
                }
                break;
            }

            if (peek() != ']')
                throw std::runtime_error("Expected ']' to close the matrix");
            pos++;

            return Matrix(rows); /* throws if row lengths are inconsistent */
        }

        Value parseExpr()
        {
            Value value = parseTerm();

            while (true)
            {
                char c = peek();

                if (c == '+')
                {
                    pos++;
                    value = valueAdd(value, parseTerm());
                }
                else if (c == '-')
                {
                    pos++;
                    value = valueSub(value, parseTerm());
                }
                else
                {
                    break;
                }
            }

            return value;
        }

        Value parseTerm()
        {
            Value value = parseFactor();

            while (peek() == '*')
            {
                pos++;
                value = valueMul(value, parseFactor());
            }

            return value;
        }

        Value parseFactor()
        {
            char c = peek();

            if (c == '-')
            {
                pos++;
                return valueNegate(parseFactor());
            }

            if (c == '+')
            {
                pos++;
                return parseFactor();
            }

            if (c == '(')
            {
                pos++;
                Value value = parseExpr();

                if (peek() != ')')
                    throw std::runtime_error("Mismatched parentheses");

                pos++;
                return value;
            }

            if (c == '[')
                return Value(parseMatrixLiteral());

            if (std::isalpha((unsigned char)c))
            {
                std::string name;

                while (pos < src.size() && std::isalpha((unsigned char)src[pos]))
                    name += src[pos++];

                if (name == "det" || name == "inverse" || name == "transpose")
                {
                    if (peek() != '(')
                        throw std::runtime_error("Expected '(' after " + name);

                    pos++;
                    Value arg = parseExpr();

                    if (peek() != ')')
                        throw std::runtime_error("Mismatched parentheses");

                    pos++;

                    if (arg.isScalar)
                        throw std::runtime_error(name + "() requires a matrix argument");

                    if (name == "det")
                        return Value(arg.matrix.determinant());
                    if (name == "inverse")
                        return Value(arg.matrix.inverse());
                    return Value(arg.matrix.transpose());
                }

                throw std::runtime_error("Unknown identifier '" + name + "'");
            }

            if (std::isdigit((unsigned char)c) || c == '.')
                return Value(parseNumber());

            throw std::runtime_error("Operand expected");
        }
    };

} /* anonymous namespace */

int evaluateMatrixExpression(const char *expr, char *result, int resultSize)
{
    try
    {
        std::string input(expr);
        MatrixParser parser(input);
        Value value = parser.parse();
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