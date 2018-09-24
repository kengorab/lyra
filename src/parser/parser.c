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

static Node* parseExpression(Parser* parser, ParseError** outErr);

// ------------------------------------
//             Statements
// ------------------------------------

static Node* parseValDeclStmt(Parser* parser, ParseError** outErr) {
    Token* valToken = advance(parser); // Consume "val" token
    if (PEEK(parser)->type != TOKEN_IDENT) {
        *outErr = newParseError(PEEK(parser), 1, TOKEN_IDENT);
        return NULL;
    }
    Token* identTok = advance(parser);

    if (PEEK(parser)->type != TOKEN_EQ) {
        *outErr = newParseError(PEEK(parser), 1, TOKEN_EQ);
        return NULL;
    }
    advance(parser); // Skip "="

    Node* rhs = parseExpression(parser, outErr);
    if (rhs == NULL) {
        if (*outErr == NULL)
            *outErr = newParseError(PEEK(parser), 0, "expression");
        return NULL;
    }

    return newValDeclStmtNode(valToken, newIdentifierNode(identTok), rhs, false);
}

static Node* parseVarDeclStmt(Parser* parser, ParseError** outErr) {
    Token* varToken = advance(parser); // Consume "var" token
    if (PEEK(parser)->type != TOKEN_IDENT) {
        *outErr = newParseError(PEEK(parser), 1, TOKEN_IDENT);
        return NULL;
    }
    Token* identTok = advance(parser);

    // Var declaration statements don't require an assignment
    if (PEEK(parser)->type != TOKEN_EQ) {
        return newValDeclStmtNode(varToken, newIdentifierNode(identTok), NULL, true);
    }

    advance(parser); // Skip "="

    Node* rhs = parseExpression(parser, outErr);
    if (rhs == NULL) {
        if (*outErr == NULL)
            *outErr = newParseError(PEEK(parser), 0, "expression");
        return NULL;
    }

    return newValDeclStmtNode(varToken, newIdentifierNode(identTok), rhs, true);
}

static Node* parseStatement(Parser* parser, ParseError** outErr) {
    Token* token = PEEK(parser);
    switch (token->type) {
        case TOKEN_VAL: return parseValDeclStmt(parser, outErr);
        case TOKEN_VAR: return parseVarDeclStmt(parser, outErr);
        default: return parseExpression(parser, outErr);
    }
}

// ------------------------------------
//             Expressions
// ------------------------------------

#ifndef GET_RULE
#define GET_RULE(tokenType) (&parseRules[tokenType])
#endif

static Node* parseUnary(Parser* parser, Token** token, ParseError** outErr);

static Node* parseBinary(Parser* parser, Token** opToken, Node** lExpr, ParseError** outErr);

static Node* parseLiteral(Parser* parser, Token** token, ParseError** outErr);

static Node* parseIdentifier(Parser* parser, Token** token, ParseError** outErr);

static Node* parseArray(Parser* parser, Token** token, ParseError** outErr);

//static Node* parseObject(Parser* parser, Token** token, ParseError** outErr);
static Node* handleParseLBrace(Parser* parser, Token** token, ParseError** outErr);

static Node* parseGrouping(Parser* parser, Token** token, ParseError** outErr);

static Node* parseIfElseExpr(Parser* parser, Token** token, ParseError** outErr);

ParseRule parseRules[] = { // These rules NEED to stay in Token order
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},             // TOKEN_NUMBER
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_ADDITION},          // TOKEN_PLUS
    {.infixFn = parseBinary, .prefixFn = parseUnary, .precedence = PREC_ADDITION},    // TOKEN_MINUS
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_MULTIPLICATION},    // TOKEN_STAR
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_MULTIPLICATION},    // TOKEN_SLASH
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_COMPARISON},        // TOKEN_LT
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_COMPARISON},        // TOKEN_LTE
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_COMPARISON},        // TOKEN_GT
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_COMPARISON},        // TOKEN_GTE
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_COMPARISON},        // TOKEN_EQ
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_EQUALITY},          // TOKEN_EQ_EQ
    {.infixFn = NULL, .prefixFn = parseUnary, .precedence = PREC_NONE},               // TOKEN_BANG
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_EQUALITY},          // TOKEN_BANG_EQ
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_AND},               // TOKEN_AND
    {.infixFn = parseBinary, .prefixFn = NULL, .precedence = PREC_OR},                // TOKEN_OR
    {.infixFn = NULL, .prefixFn = parseArray, .precedence = PREC_NONE},               // TOKEN_LBRACK
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_RBRACK
    {.infixFn = NULL, .prefixFn = handleParseLBrace, .precedence = PREC_NONE},        // TOKEN_LBRACE
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_RBRACE
    {.infixFn = NULL, .prefixFn = parseGrouping, .precedence = PREC_NONE},            // TOKEN_LPAREN
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_RPAREN
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_DOT
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_COMMA
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_COLON
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_COLON_COLON
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_ARROW
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_PIPE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},             // TOKEN_STRING
    {.infixFn = NULL, .prefixFn = parseIdentifier, .precedence = PREC_NONE},          // TOKEN_IDENT
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_VAL
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_VAR
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_TYPE
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_FUNC
    {.infixFn = NULL, .prefixFn = parseIfElseExpr, .precedence = PREC_NONE},          // TOKEN_IF
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_ELSE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},             // TOKEN_TRUE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},             // TOKEN_FALSE
    {.infixFn = NULL, .prefixFn = parseLiteral, .precedence = PREC_NONE},             // TOKEN_NIL
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_ERROR
    {.infixFn = NULL, .prefixFn = NULL, .precedence = PREC_NONE},                     // TOKEN_EOF
};

static Node* parsePrecedence(Parser* parser, Precedence precedence, ParseError** outErr) {
    Token* prefixToken = *parser->current;

    ParseRule* rule = GET_RULE(prefixToken->type);
    if (rule == NULL || rule->prefixFn == NULL) {
//        printf("Error, no prefix rule for tokenType: %s\n", tokenTypes[prefixToken->type]);
        return NULL;
    }

    // Cursor sits on token AFTER the match token; in-/prefix fns always start on token AFTER the one they match
    advance(parser);

    Node* left = rule->prefixFn(parser, &prefixToken, outErr);

    while (precedence <= GET_RULE((*parser->current)->type)->precedence) {
        Token* infixToken = *parser->current;
        advance(parser); // See comment above for prefix token
        rule = GET_RULE(infixToken->type);
        if (rule == NULL || rule->infixFn == NULL) {
//            printf("Error, no infix rule for tokenType: %s\n", tokenTypes[infixToken->type]);
            return NULL;
        }

        InfixFn infixFn = rule->infixFn;
        left = infixFn(parser, &infixToken, &left, outErr);
    }

    return left;
}

static Node* parseLiteral(Parser* parser, Token** token, ParseError** outErr) {
    return newLiteralNode(*token);
}

static Node* parseIdentifier(Parser* parser, Token** token, ParseError** outErr) {
    return newIdentifierNode(*token);
}

static Node* parseUnary(Parser* parser, Token** token, ParseError** outErr) {
    Node* expr = parsePrecedence(parser, PREC_UNARY, outErr);
    return newUnaryNode(*token, expr);
}

static Node* parseBinary(Parser* parser, Token** opToken, Node** lExpr, ParseError** outErr) {
    Precedence prec;
    switch ((*opToken)->type) {
        case TOKEN_MINUS:
        case TOKEN_PLUS: {
            prec = PREC_ADDITION;
            break;
        }
        case TOKEN_STAR:
        case TOKEN_SLASH: {
            prec = PREC_MULTIPLICATION;
            break;
        }
        case TOKEN_LT:
        case TOKEN_LTE:
        case TOKEN_GT:
        case TOKEN_GTE: {
            prec = PREC_COMPARISON;
            break;
        }
        case TOKEN_EQ_EQ:
        case TOKEN_BANG_EQ: {
            prec = PREC_EQUALITY;
            break;
        }
        case TOKEN_AND: {
            prec = PREC_AND;
            break;
        }
        case TOKEN_OR: {
            prec = PREC_OR;
            break;
        }
        default: {
            // This is an invalid state, and should be unreachable
            return NULL;
        }
    }

    Node* rExpr = parsePrecedence(parser, prec, outErr);
    return newBinaryNode(*opToken, *lExpr, rExpr);
}

static Node* parseArray(Parser* parser, Token** token, ParseError** outErr) {
    List* elements = newList();
    while (PEEK(parser)->type != TOKEN_RBRACK && !IS_AT_END(parser)) {
        Node* elem = parseExpression(parser, outErr);
        listAdd(elements, (void**) &elem);
        if (PEEK(parser)->type == TOKEN_COMMA)
            advance(parser); // Consume the ","
        else if (PEEK(parser)->type != TOKEN_RBRACK) {
            *outErr = newParseError(PEEK(parser), 2, TOKEN_COMMA, TOKEN_RBRACK);
            return NULL;
        }
    }
    advance(parser); // Consume the "]"

    Node** arrayElements = (Node**) elements->values;
    return newArrayLiteralNode(*token, arrayElements, elements->count);
}

static Node* parseObject(Parser* parser, Token** token, ParseError** outErr) {
    List* keys = newList();
    List* entries = newList();
    while (!IS_AT_END(parser) && PEEK(parser)->type != TOKEN_RBRACE) {
        if (PEEK(parser)->type != TOKEN_IDENT) {
            *outErr = newParseError(PEEK(parser), 1, TOKEN_IDENT);
            return NULL;
        }
        Token* keyToken = advance(parser);
        Node* key = parseIdentifier(parser, &keyToken, outErr);
        listAdd(keys, (void**) &key);

        if (PEEK(parser)->type != TOKEN_COLON) {
            *outErr = newParseError(PEEK(parser), 1, TOKEN_COLON);
            return NULL;
        }
        advance(parser); // Consume ":"

        Node* value = parseExpression(parser, outErr);
        if (value == NULL) {
            if (*outErr == NULL)
                *outErr = newParseError(PEEK(parser), 0, "expression");
            return NULL;
        }

        if (PEEK(parser)->type == TOKEN_COMMA)
            advance(parser); // Consume the ","
        else if (PEEK(parser)->type != TOKEN_RBRACE) {
            if (*outErr == NULL)
                *outErr = newParseError(PEEK(parser), 2, TOKEN_COMMA, TOKEN_RBRACE);
            return NULL;
        }

        ObjectLiteralEntry* entry = newObjectLiteralEntry(key, value);
        listAdd(entries, (void**) &entry);
    }
    advance(parser); // Consume "}"

    return newObjectLiteralNode(*token, (ObjectLiteralEntry**) entries->values, (Node**) keys->values, entries->count);
}

static Node* parseBlock(Parser* parser, Token** token, ParseError** outErr) {
    List* exprStmts = newList();
    while (!IS_AT_END(parser) && PEEK(parser)->type != TOKEN_RBRACE) {
        Node* exprStmt = parseStatement(parser, outErr);
        listAdd(exprStmts, (void**) &exprStmt);
    }
    advance(parser); // Consume "}"
    return newBlockNode(*token, (Node**) exprStmts->values, exprStmts->count);
}

static Node* handleParseLBrace(Parser* parser, Token** token, ParseError** outErr) {
    if (!IS_AT_END(parser) && PEEK(parser)->type == TOKEN_RBRACE) {
        advance(parser); // Consume "}"
        return newObjectLiteralNode(*token, NULL, NULL, 0);
    } else if (!IS_AT_END(parser) && PEEK(parser)->type == TOKEN_IDENT && PEEK_NEXT(parser)->type == TOKEN_COLON) {
        return parseObject(parser, token, outErr);
    } else if (!IS_AT_END(parser) && PEEK(parser)->type == TOKEN_COLON) {
        *outErr = newParseError(PEEK(parser), 1, TOKEN_IDENT);
        return NULL;
    } else {
        return parseBlock(parser, token, outErr);
    }
}

static Node* parseGrouping(Parser* parser, Token** token, ParseError** outErr) {
    Node* expr = parseExpression(parser, outErr);
    advance(parser); // Consume the ")"
    return newGroupingNode(*token, expr);
}

static Node* parseIfElseExpr(Parser* parser, Token** token, ParseError** outErr) {
    if (PEEK(parser)->type != TOKEN_LPAREN) {
        if (*outErr == NULL)
            *outErr = newParseError(PEEK(parser), 1, TOKEN_LPAREN);
        return NULL;
    }
    advance(parser); // Consume "("

    Node* condExpr = parseExpression(parser, outErr);
    if (condExpr == NULL) {
        if (*outErr == NULL)
            *outErr = newParseError(PEEK(parser), 0, "expression");
        return NULL;
    }

    if (PEEK(parser)->type != TOKEN_RPAREN) {
        if (*outErr == NULL)
            *outErr = newParseError(PEEK(parser), 1, TOKEN_RPAREN);
        return NULL;
    }
    advance(parser); // Consume ")"

    // TODO: Handle block expressions here

    Node* thenExpr = parseExpression(parser, outErr);
    if (thenExpr == NULL) {
        if (*outErr == NULL)
            *outErr = newParseError(PEEK(parser), 0, "expression");
        return NULL;
    }

    Node* elseExpr = NULL;
    if (PEEK(parser)->type == TOKEN_ELSE) {
        advance(parser); // Consume "else"
        elseExpr = parseExpression(parser, outErr);
        if (elseExpr == NULL) {
            if (*outErr == NULL)
                *outErr = newParseError(PEEK(parser), 0, "expression");
            return NULL;
        }
    }

    return newIfElseNode(*token, condExpr, thenExpr, elseExpr);
}

static Node* parseExpression(Parser* parser, ParseError** outErr) {
    return parsePrecedence(parser, PREC_ASSIGNMENT, outErr); // Start with lowest precedence
}

static void skipToNextSafeZone(Parser* parser) {
    while (!IS_AT_END(parser) && PEEK(parser)->type != TOKEN_VAL)
        advance(parser);
}

List* parse(Parser* parser, List** outErrList) {
    List* statements = newList();

    while (!IS_AT_END(parser)) {
        ParseError* error = NULL;
        Node* stmt = parseStatement(parser, &error);
        if (error == NULL) {
            listAdd(statements, (void**) &stmt);
        } else {
            listAdd(*outErrList, (void**) &error);
            skipToNextSafeZone(parser);
        }
    }

    return statements;
}
