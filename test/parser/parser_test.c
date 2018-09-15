#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <parser/ast.h>

#include "parser_test.h"
#include "parser/ast.h"
#include "parser/parser.h"

static Token* makeToken(const char* contents, TokenType type);

static TestResult assertLiteralNode(const char* testName, Node* n, LiteralNodeType litType, ...);

TEST(testParseIntLiteral, {
    Token** tokens = ((Token* []) {
        makeToken("1", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_INT, 1);
})

TEST(testParseDoubleLiteral, {
    Token** tokens = ((Token* []) {
        makeToken("1.23", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_DOUBLE, 1.23);
})

TEST(testParseBoolLiterals, {
    Token** tokens = ((Token* []) {
        makeToken("true", TOKEN_TRUE),
        makeToken("false", TOKEN_FALSE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(2, nodes->count, "There should be 2 elements in the list");

    TestResult res = assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;
    else return assertLiteralNode(testName, nodes->values[1], LITERAL_NODE_BOOL, false);
})

TEST(testParseStringLiteral, {
    Token** tokens = ((Token* []) {
        makeToken("\"some string\"", TOKEN_STRING),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];

    return assertLiteralNode(testName, n, LITERAL_NODE_STRING, "\"some string\"", 13);
})

TEST(testParseNilLiteral, {
    Token** tokens = ((Token* []) {
        makeToken("nil", TOKEN_NIL),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_NIL);
})

TEST(testParseValDeclStatement, {
    Token** tokens = ((Token* []) {
        makeToken("val", TOKEN_VAL),
        makeToken("someValue", TOKEN_IDENT),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue", valDeclStmt->ident->name, "The ident should be someValue");
    return assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_INT, 123);
})

TestResult testParseValDeclStatements() {
    const char* testName = "testParseValDeclStatements";
    Token** tokens = ((Token* []) {
        makeToken("val", TOKEN_VAL),
        makeToken("someValue", TOKEN_IDENT),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("val", TOKEN_VAL),
        makeToken("someValue2", TOKEN_IDENT),
        makeToken("=", TOKEN_EQ),
        makeToken("\"hello!\"", TOKEN_STRING),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(2, nodes->count, "There should be 2 elements in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue", valDeclStmt->ident->name, "The ident should be someValue");
    TestResult res = assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_INT, 123);
    if (!res.pass) return res;

    n = nodes->values[1];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue2", valDeclStmt->ident->name, "The ident should be someValue2");
    return assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_STRING, "\"hello!\"", 8);
}

void runParserTests(Tester* tester) {
    tester->startSuite("Parser");
    tester->run(testParseIntLiteral);
    tester->run(testParseDoubleLiteral);
    tester->run(testParseBoolLiterals);
    tester->run(testParseStringLiteral);
    tester->run(testParseNilLiteral);
    tester->run(testParseValDeclStatement);
    tester->run(testParseValDeclStatements);
}

static Token* makeToken(const char* contents, TokenType type) {
    Token* t = malloc(sizeof(Token));
    t->start = contents;
    t->length = (int) strlen(contents);
    t->type = type;
    t->col = 1;
    t->line = 1;
    return t;
}

static TestResult assertLiteralNode(const char* testName, Node* n, LiteralNodeType litType, ...) {
    va_list args;
    va_start(args, litType);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;

    char* msg = malloc(100);
    sprintf(msg, "The literal node should be of type %s", literalNodeTypes[litType]);
    ASSERT_EQ_STR(literalNodeTypes[litType], literalNodeTypes[literalNode->type], msg);

    switch (litType) {
        case LITERAL_NODE_STRING: {
            char* str = va_arg(args, char*);
            msg = malloc(100);
            sprintf(msg, "The literal node should contain str.val=%s", str);
            ASSERT_EQ_STR(str, literalNode->str.val, msg);

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
            bool val = va_arg(args, bool);
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
