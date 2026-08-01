/**
 * @file token.h
 * @brief Token representation shared by the tokenizer/parser stages.
 */
#ifndef TOKEN_H
#define TOKEN_H

/** Maximum length (including the null terminator) of a token's text. */
#define TOKEN_TEXT_SIZE 32

/** Kinds of lexical tokens the parser can produce. */
typedef enum
{
    TOKEN_NUMBER,      /**< A numeric literal, e.g. "3.14". */
    TOKEN_OPERATOR,    /**< An operator, e.g. "+", "*", "!". */
    TOKEN_FUNCTION,    /**< A function name, e.g. "sin". */
    TOKEN_VARIABLE,    /**< A variable or constant name, e.g. "x", "pi". */
    TOKEN_LEFT_PAREN,  /**< "(" */
    TOKEN_RIGHT_PAREN, /**< ")" */
    TOKEN_COMMA        /**< "," (argument separator). */
} TokenType;

/** A single lexical token. */
typedef struct
{
    TokenType type;             /**< What kind of token this is. */
    char text[TOKEN_TEXT_SIZE]; /**< The token's raw text. */
} Token;

#endif
