#ifndef CLYRA_LEXER_H
#define CLYRA_LEXER_H

#include "common/enums.h"

MAKE_ENUM(
    TokenType,
    tokenTypes,

    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LT,
    TOKEN_LTE,
    TOKEN_GT,
    TOKEN_GTE,
    TOKEN_EQ,
    TOKEN_EQ_EQ,
    TOKEN_BANG,
    TOKEN_BANG_EQ,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LBRACK,
    TOKEN_RBRACK,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_COLON_COLON,
    TOKEN_ARROW,
    TOKEN_PIPE,
    TOKEN_STRING,
    TOKEN_IDENT,
    TOKEN_VAL,
    TOKEN_VAR,
    TOKEN_TYPE,
    TOKEN_FUNC,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NIL,
    TOKEN_ERROR,
    TOKEN_EOF
)

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
    int col;
} Token;

const char* tokenGetValue(Token* token);

#define PRINT_TOKEN(pToken) \
    printf("Token { type: %s, line: %d, col: %d, contents: \"%.*s\" }\n",\
           tokenTypes[pToken->type], pToken->line, pToken->col, pToken->length, pToken->start)

typedef struct {
    char* start;
    char* current;
    int line;
    int col;
} Lexer;

Lexer newLexer(char* source);

Token* nextToken(Lexer* lexer);

#endif //CLYRA_LEXER_H
