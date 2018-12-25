#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "test_utils.h"
#include "common/strings.h"
#include "parser/ast.h"

Token* makeToken(const char* contents, TokenType type) {
    Token* t = malloc(sizeof(Token));
    t->start = contents;
    t->length = (int) strlen(contents);
    t->type = type;
    t->col = 1;
    t->line = 1;
    return t;
}

Parser parseString(char* sourceStr) {
    Lexer l = newLexer(sourceStr);

    List* tokenList = newList();
    while (true) {
        Token* t = nextToken(&l);
        listAdd(tokenList, (void**) &t);

        if (t->type == TOKEN_EOF)
            break;
    }

    return newParser((Token**) tokenList->values);
}

TestResult assertIdentNode(const char* testName, Node* n, const char* name) {
    ASSERT_EQ_STR("NODE_TYPE_IDENT", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_IDENT");

    IdentifierNode* ident = n->as.identifierNode;
    char* identName = substring(ident->token->start, (size_t) ident->token->length);
    ASSERT_EQ_STR(name, identName, "The ident name should match");
    PASS;
}

TestResult assertLiteralNode(const char* testName, Node* n, LiteralNodeType litType, ...) {
    va_list args;
    va_start(args, litType);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;

    char* msg = malloc(100);
    sprintf(msg, "The literal node should be of type %s", literalNodeTypes[litType]);
    ASSERT_EQ_STR(literalNodeTypes[litType], literalNodeTypes[literalNode->type], msg);

    switch (litType) {
        case LITERAL_NODE_STRING: {
            char* str = va_arg(args, char*);
            msg = malloc(100);
            sprintf(msg, "The literal node should contain str.val=%s", str);
            char* literalStr = substring(literalNode->str.val, (size_t) literalNode->str.length);
            ASSERT_EQ_STR(str, literalStr, msg);

            int len = va_arg(args, int);
            msg = malloc(100);
            sprintf(msg, "The literal node should contain str.length=%d", len);
            ASSERT_EQ(len, literalNode->str.length, msg);
            break;
        }
        case LITERAL_NODE_INT: {
            int val = va_arg(args, int);
            msg = malloc(100);
            sprintf(msg, "The literal node should contain iVal=%d", val);
            ASSERT_EQ(val, literalNode->iVal, msg);
            break;
        }
        case LITERAL_NODE_DOUBLE: {
            double val = va_arg(args, double);
            msg = malloc(100);
            sprintf(msg, "The literal node should contain iVal=%f", val);
            ASSERT_EQ(val, literalNode->dVal, msg);
            break;
        }
        case LITERAL_NODE_BOOL: {
            bool val = va_arg(args, int) == 1;
            msg = malloc(100);
            sprintf(msg, "The literal node should contain bVal=%s", val ? "true" : "false");
            ASSERT_EQ(val, literalNode->bVal, msg);
            break;
        }
        case LITERAL_NODE_NIL:
        default: {
            // Nothing to do here
        }
    }

    va_end(args);
    PASS;
}
