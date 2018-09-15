#include <stdlib.h>
#include <stdio.h>

#include "parser/parser.h"

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

static Node* parseExpression(Parser* parser);

// ------------------------------------
//             Statements
// ------------------------------------

static Node* parseValDeclStmt(Parser* parser, Token* valToken) {
    advance(parser); // Consume "val" token
    if (PEEK(parser)->type != TOKEN_IDENT)
        return NULL; // TODO: Parser error handling
    Token* identTok = advance(parser);

    if (PEEK(parser)->type != TOKEN_EQ)
        return NULL; // TODO: Parser error handling
    advance(parser); // Skip "="

    Node* rhs = parseExpression(parser);

    return newValDeclStmtNode(valToken, newIdentifierNode(identTok), rhs);
}

static Node* parseStatement(Parser* parser) {
    Token* token = PEEK(parser);
    switch (token->type) {
        case TOKEN_VAL: return parseValDeclStmt(parser, token);
        default: return parseExpression(parser);
    }
}

// ------------------------------------
//             Expressions
// ------------------------------------

#ifndef GET_RULE
#define GET_RULE(tokenType) (&parseRules[tokenType])
#endif

static Node* parseLiteral(Parser* parser, Token** token);

ParseRule parseRules[] = { // These rules NEED to stay in Token order
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},           // TOKEN_NUMBER
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_ADDITION},               // TOKEN_PLUS
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_ADDITION},               // TOKEN_MINUS
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_MULTIPLICATION},         // TOKEN_STAR
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_MULTIPLICATION},         // TOKEN_SLASH
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_LT
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_LTE
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_GT
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_GTE
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_EQ
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_EQ_EQ
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_BANG
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_COMPARISON},             // TOKEN_BANG_EQ
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_AND},                    // TOKEN_AND
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_OR},                     // TOKEN_OR
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_LBRACK
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_RBRACK
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_LBRACE
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_RBRACE
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_LPAREN
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_RPAREN
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_DOT
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_COMMA
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_COLON
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_COLON_COLON
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_ARROW
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_PIPE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},           // TOKEN_STRING
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_IDENT
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_VAL
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_VAR
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_TYPE
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_FUNC
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_IF
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_ELSE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},           // TOKEN_TRUE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},           // TOKEN_FALSE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},           // TOKEN_NIL
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_ERROR
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                   // TOKEN_EOF
};

static Node* parseLiteral(Parser* parser, Token** token) {
    return newLiteralNode(*token);
}

static Node* parsePrecedence(Parser* parser, Precedence precedence) {
    Token* prefixToken = *parser->current;

    // Cursor sits on token AFTER the match token; in-/prefix fns always start on token AFTER the one they match
    advance(parser);
    ParseRule* rule = GET_RULE(prefixToken->type);
    if (rule == NULL || rule->prefixFn == NULL) {
        printf("Error, no prefix rule for tokenType: %s\n", tokenTypes[prefixToken->type]);
        return NULL;
    }

    Node* left = rule->prefixFn(parser, &prefixToken);

    while (precedence <= GET_RULE((*parser->current)->type)->precedence) {
        Token* infixToken = *parser->current;
        advance(parser); // See comment above for prefix token
        InfixFn infixFn = GET_RULE(infixToken->type)->infixFn;
        left = infixFn(parser, &infixToken, &left);
    }

    return left;
}

static Node* parseExpression(Parser* parser) {
    return parsePrecedence(parser, PREC_ASSIGNMENT); // Start with lowest precedence
}

List* parse(Parser* parser) {
    List* statements = newList();

    while (!IS_AT_END(parser)) {
        Node* stmt = parseStatement(parser);
        listAdd(statements, (void**) &stmt);
    }

    return statements;
}
