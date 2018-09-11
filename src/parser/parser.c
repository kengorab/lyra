#include <stdlib.h>

#include "parser.h"

/* Functions/macros for moving the cursor around */
#define PEEK(parser) (*parser->current)
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

Node* parseValDeclStmt(Token* valToken, Parser* parser) {
    if (PEEK(parser)->type != TOKEN_IDENT)
        return NULL; // TODO: Parser error handling
    Token* identTok = advance(parser);

    if (PEEK(parser)->type != TOKEN_EQ)
        return NULL; // TODO: Parser error handling
    advance(parser); // Skip "="
    advance(parser); // Skip expr (don't care for now)

    return newValDeclStmtNode(valToken, newIdentifierNode(identTok), NULL);
}

Node* parse(Parser* parser) {
    Token* token = advance(parser);

    switch (token->type) {
        case TOKEN_VAL: {
            return parseValDeclStmt(token, parser);
        }
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
