#ifndef CLYRA_LEXER_H
#define CLYRA_LEXER_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

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
