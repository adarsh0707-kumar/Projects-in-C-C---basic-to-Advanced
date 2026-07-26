#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_TEXT_SIZE 32

typedef enum
{
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_FUNCTION,
    TOKEN_VARIABLE,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_COMMA
} TokenType;

typedef struct
{
    TokenType type;
    char text[TOKEN_TEXT_SIZE];
} Token;

#endif