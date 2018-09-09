#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"

/* Functions/macros for moving the cursor around */
#define PEEK(lexer) *lexer->current
#define IS_AT_END(lexer) (PEEK(lexer) == 0)
#define PEEK_NEXT(lexer) (IS_AT_END(lexer) ? 0 : lexer->current[1])

static char advance(Lexer* lexer) {
    lexer->current++;
    lexer->col++;
    return lexer->current[-1];
}

/* Helpers */
#define IS_DIGIT(ch) ('0' <= ch && ch <= '9')
#define IS_ALPHA(ch) ('a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z')

const char* tokenTypes[] = {TOKEN_TYPES};

Lexer newLexer(char* source) {
    Lexer l = {
        .current = source,
        .start = source,
        .col = 0, // Initialize col to 0, it'll be incremented at first advance()
        .line = 1
    };
    return l;
}

static bool match(Lexer* lexer, char expected) {
    if (IS_AT_END(lexer)) return false;
    if (*lexer->current != expected) return false;

    lexer->current++;
    lexer->col++;
    return true;
}

static void skipWhitespace(Lexer* lexer) {
    for (;;) {
        char c = PEEK(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t': {
                advance(lexer);
                break;
            }
            case '\n': {
                lexer->line++;
                lexer->col = -1; // Reset col to -1, it'll be incremented twice (next line, and next advance())
                advance(lexer);
                break;
            }
            case '/': {
                if (PEEK_NEXT(lexer) == '/') {
                    while (PEEK(lexer) != '\n' && !IS_AT_END(lexer))
                        advance(lexer);
                } else {
                    return;
                }
            }
            default: {
                return;
            }
        }

        return;
    }
}

static Token newToken(TokenType type, const Lexer* lexer, int col) {
    Token t = {
        .type = type,
        .start = lexer->start,
        .length = (int) (lexer->current - lexer->start),
        .line = lexer->line,
        .col = col
    };
    return t;
}

static Token newSingleCharToken(TokenType type, const Lexer* lexer) {
    return newToken(type, lexer, lexer->col);
}

static Token errorToken(const char* message, const Lexer* lexer, int col) {
    Token t = {
        .type = TOKEN_ERROR,
        .start = message,
        .length = (int) strlen(message),
        .line = lexer->line,
        .col = col
    };
    return t;
}

static Token unexpectedCharError(Lexer* lexer, char ch, int col) {
    char msg[24];
    sprintf(msg, "Unexpected character: %c", ch);
    return errorToken(msg, lexer, col);
}

static bool verifyKeyword(Lexer* lexer, const char* keyword) {
    size_t keywordLen = strlen(keyword);
    if (lexer->current - lexer->start != keywordLen) {
        return false;
    }

    if (memcmp(lexer->start, keyword, keywordLen) == 0) {
        return true;
    }

    return false;
}

static TokenType tokenizeIdentOrKeyword(Lexer* lexer) {

#define CASE(kwd, tok) \
    if (verifyKeyword(lexer, kwd)) {\
        return tok;\
    }

    CASE("val", TOKEN_VAL);
    CASE("var", TOKEN_VAR);
    CASE("type", TOKEN_TYPE);
    CASE("func", TOKEN_FUNC);
    CASE("if", TOKEN_IF);
    CASE("else", TOKEN_ELSE);
    CASE("true", TOKEN_TRUE);
    CASE("false", TOKEN_FALSE);
    CASE("nil", TOKEN_NIL);

    return TOKEN_IDENT;
}

Token nextToken(Lexer* lexer) {
    // Skip the cursor (lexer->current) ahead, then reposition lexer->start
    skipWhitespace(lexer);
    lexer->start = lexer->current;

    if (IS_AT_END(lexer)) return newSingleCharToken(TOKEN_EOF, lexer);

    char c = advance(lexer);

    if (IS_DIGIT(c)) {
        int col = lexer->col;
        while (IS_DIGIT(PEEK(lexer)))
            advance(lexer);
        if (PEEK(lexer) == '.' && IS_DIGIT(PEEK_NEXT(lexer))) {
            advance(lexer); // Consume the '.'

            while (IS_DIGIT(PEEK(lexer)))
                advance(lexer);
        }
        return newToken(TOKEN_NUMBER, lexer, col);
    }

    if (IS_ALPHA(c)) {
        int startCol = lexer->col;
        while (IS_ALPHA(PEEK(lexer)) || IS_DIGIT(PEEK(lexer)))
            advance(lexer);
        return newToken(tokenizeIdentOrKeyword(lexer), lexer, startCol);
    }

    switch (c) {
#define QUOTE_CASE(quote) \
        case quote: { \
            int colStart = lexer->col; \
            while (PEEK(lexer) != quote && !IS_AT_END(lexer)) { \
                if (PEEK(lexer) == '\n') return errorToken("Unterminated string", lexer, colStart); \
                advance(lexer); \
            } \
            if (IS_AT_END(lexer)) return errorToken("Unterminated string", lexer, colStart); \
                advance(lexer); \
            return newToken(TOKEN_STRING, lexer, colStart); \
        }

        QUOTE_CASE('"')
        QUOTE_CASE('\'')
        case '+': return newSingleCharToken(TOKEN_PLUS, lexer);
        case '-': return newSingleCharToken(TOKEN_MINUS, lexer);
        case '*': return newSingleCharToken(TOKEN_STAR, lexer);
        case '/': return newSingleCharToken(TOKEN_SLASH, lexer);
        case '<': {
            if (match(lexer, '=')) return newToken(TOKEN_LTE, lexer, lexer->col - 1);
            else return newSingleCharToken(TOKEN_LT, lexer);
        }
        case '>': {
            if (match(lexer, '=')) return newToken(TOKEN_GTE, lexer, lexer->col - 1);
            else return newSingleCharToken(TOKEN_GT, lexer);
        }
        case '!': {
            if (match(lexer, '=')) return newToken(TOKEN_BANG_EQ, lexer, lexer->col - 1);
            else return newSingleCharToken(TOKEN_BANG, lexer);
        }
        case '=': {
            if (match(lexer, '=')) return newToken(TOKEN_EQ_EQ, lexer, lexer->col - 1);
            if (match(lexer, '>')) return newToken(TOKEN_ARROW, lexer, lexer->col - 1);
            else return newSingleCharToken(TOKEN_EQ, lexer);
        }
        case '&': {
            if (match(lexer, '&')) return newToken(TOKEN_AND, lexer, lexer->col - 1);
            else {
                advance(lexer);
                return unexpectedCharError(lexer, c, lexer->col - 1);
            }
        }
        case '|': {
            if (PEEK(lexer) == '|') {
                advance(lexer); // Consume "|"
                return newToken(TOKEN_OR, lexer, lexer->col - 1);
            } else return newSingleCharToken(TOKEN_PIPE, lexer);
        }
        case '[': return newSingleCharToken(TOKEN_LBRACK, lexer);
        case ']': return newSingleCharToken(TOKEN_RBRACK, lexer);
        case '{': return newSingleCharToken(TOKEN_LBRACE, lexer);
        case '}': return newSingleCharToken(TOKEN_RBRACE, lexer);
        case '(': return newSingleCharToken(TOKEN_LPAREN, lexer);
        case ')': return newSingleCharToken(TOKEN_RPAREN, lexer);
        case '.': return newSingleCharToken(TOKEN_DOT, lexer);
        case ',': return newSingleCharToken(TOKEN_COMMA, lexer);
        case ':': {
            if (match(lexer, ':')) return newToken(TOKEN_COLON_COLON, lexer, lexer->col - 1);
            else return newSingleCharToken(TOKEN_COLON, lexer);
        }

        default: return unexpectedCharError(lexer, c, lexer->col);
    }
}
