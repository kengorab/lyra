#ifndef CLYRA_LEXER_H
#define CLYRA_LEXER_H

#define C(ENUM_VAL) ENUM_VAL,
#define TOKEN_TYPES \
    C(TOKEN_NUMBER) \
    C(TOKEN_PLUS) \
    C(TOKEN_MINUS) \
    C(TOKEN_STAR) \
    C(TOKEN_SLASH) \
    C(TOKEN_LT) \
    C(TOKEN_LTE) \
    C(TOKEN_GT) \
    C(TOKEN_GTE) \
    C(TOKEN_EQ) \
    C(TOKEN_EQ_EQ) \
    C(TOKEN_BANG) \
    C(TOKEN_BANG_EQ) \
    C(TOKEN_AND) \
    C(TOKEN_OR) \
    C(TOKEN_LBRACK) \
    C(TOKEN_RBRACK) \
    C(TOKEN_LBRACE) \
    C(TOKEN_RBRACE) \
    C(TOKEN_LPAREN) \
    C(TOKEN_RPAREN) \
    C(TOKEN_DOT) \
    C(TOKEN_COMMA) \
    C(TOKEN_COLON) \
    C(TOKEN_ERROR) \
    C(TOKEN_EOF)

typedef enum {
    TOKEN_TYPES
} TokenType;

#undef C
#define C(ENUM_VAL) #ENUM_VAL,

// Ignore warning; initialized statically
const char* tokenTypes[];

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
    int col;
} Token;

#define PRINT_TOKEN(token) \
    printf("Token { type: %d, line: %d, col: %d, contents: \"%.*s\" }",\
           token.type, token.line, token.col, token.length, token.start)


typedef struct {
    char* start;
    char* current;
    int line;
    int col;
} Lexer;

Lexer newLexer(char* source);

Token nextToken(Lexer* lexer);

#endif //CLYRA_LEXER_H
