#include <stdlib.h>

#include "parser.h"

/* Functions/macros for moving the cursor around */
#define PEEK(parser) *parser->current
#define IS_AT_END(parser) (PEEK(parser)->type == TOKEN_EOF)
#define PEEK_NEXT(parser) (IS_AT_END(parser) ? 0 : parser->current[1])

static Token* advance(Parser* parser) {
    parser->current++;
    return parser->current[-1];
}

Parser newParser(Token** tokens) {
    Parser p = {
        .current = tokens,
        .start = tokens
    };
    return p;
}

Node* parse(Parser* parser) {
    Token* token = advance(parser);

    switch (token->type) {
        case TOKEN_NUMBER:
        case TOKEN_STRING:
        case TOKEN_TRUE:
        case TOKEN_FALSE: {
            return newLiteralNode(token);
        }
        case TOKEN_IDENT: return newIdentifierNode(token);
        default: return NULL;
    }
}
