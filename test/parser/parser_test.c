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

TEST(testParseIdentifier, {
    Token** tokens = ((Token* []) {
        makeToken("abc", TOKEN_IDENT),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_IDENT", astNodeTypes[n->type], "The node type should be NODE_TYPE_IDENT");
})

TEST(testUnaryExpression_minus, {
    Token** tokens = ((Token* []) {
        makeToken("-", TOKEN_MINUS),
        makeToken("1.23", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_UNARY", astNodeTypes[n->type], "The node should have type NODE_TYPE_UNARY");
    UnaryNode* unary = n->as.unaryNode;

    ASSERT_EQ_STR("-", unary->token->start, "The token should be a minus");
    return assertLiteralNode(testName, unary->expr, LITERAL_NODE_DOUBLE, 1.23);
})

TEST(testUnaryExpression_negate, {
    Token** tokens = ((Token* []) {
        makeToken("!", TOKEN_BANG),
        makeToken("true", TOKEN_TRUE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_UNARY", astNodeTypes[n->type], "The node should have type NODE_TYPE_UNARY");
    UnaryNode* unary = n->as.unaryNode;

    ASSERT_EQ_STR("!", unary->token->start, "The token should be a bang");
    return assertLiteralNode(testName, unary->expr, LITERAL_NODE_BOOL, true);
})

TEST(testBinaryExpression, {
    Token** tokens = ((Token* []) {
        makeToken("1", TOKEN_NUMBER),
        makeToken("+", TOKEN_PLUS),
        makeToken("2", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BINARY", astNodeTypes[n->type], "The node should have type NODE_TYPE_BINARY");
    BinaryNode* binary = n->as.binaryNode;

    TestResult res = assertLiteralNode(testName, binary->lExpr, LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    ASSERT_EQ_STR("+", binary->token->start, "The token should be a +");
    return assertLiteralNode(testName, binary->rExpr, LITERAL_NODE_INT, 2);
})

TEST(testBinaryExpression_precedences, {
    // 1 + 2 * 3 >= 4 && false == true
    // (((1 + (2 * 3)) >= 4) && (false == true))
    Token** tokens = ((Token* []) {
        makeToken("1", TOKEN_NUMBER),
        makeToken("+", TOKEN_PLUS),
        makeToken("2", TOKEN_NUMBER),
        makeToken("*", TOKEN_STAR),
        makeToken("3", TOKEN_NUMBER),
        makeToken(">=", TOKEN_GTE),
        makeToken("4", TOKEN_NUMBER),
        makeToken("&&", TOKEN_AND),
        makeToken("false", TOKEN_FALSE),
        makeToken("==", TOKEN_EQ_EQ),
        makeToken("true", TOKEN_TRUE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BINARY", astNodeTypes[n->type], "The node should have type NODE_TYPE_BINARY");

    // The && expr
    BinaryNode* binary = n->as.binaryNode;
    ASSERT_EQ_STR("&&", binary->token->start, "The operator should be &&");

    // The == expr
    BinaryNode* rhs = binary->rExpr->as.binaryNode;
    ASSERT_EQ_STR("==", rhs->token->start, "The operator should be ==");
    TestResult res = assertLiteralNode(testName, rhs->lExpr, LITERAL_NODE_BOOL, false);
    if (!res.pass) return res;
    res = assertLiteralNode(testName, rhs->rExpr, LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;

    // The >= expr
    binary = binary->lExpr->as.binaryNode;
    ASSERT_EQ_STR(">=", binary->token->start, "The operator should be >=");
    res = assertLiteralNode(testName, binary->rExpr, LITERAL_NODE_INT, 4);
    if (!res.pass) return res;

    // The + expr
    binary = binary->lExpr->as.binaryNode;
    ASSERT_EQ_STR("+", binary->token->start, "The operator should be +");
    res = assertLiteralNode(testName, binary->lExpr, LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    // The * expr
    binary = binary->rExpr->as.binaryNode;
    ASSERT_EQ_STR("*", binary->token->start, "The operator should be *");
    res = assertLiteralNode(testName, binary->lExpr, LITERAL_NODE_INT, 2);
    if (!res.pass) return res;
    return assertLiteralNode(testName, binary->rExpr, LITERAL_NODE_INT, 3);
})

TEST(testArrayLiteralExpression, {
    Token** tokens = ((Token* []) {
        makeToken("[", TOKEN_LBRACK),
        makeToken("1", TOKEN_NUMBER),
        makeToken(",", TOKEN_COMMA),
        makeToken("2", TOKEN_NUMBER),
        makeToken("]", TOKEN_RBRACK),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_ARRAY_LITERAL", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_ARRAY_LITERAL");
    ArrayLiteralNode* array = n->as.arrayLiteralNode;

    TestResult res = assertLiteralNode(testName, array->elements[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;
    return assertLiteralNode(testName, array->elements[1], LITERAL_NODE_INT, 2);
})

TEST(testArrayLiteralExpression_trailingCommas, {
    Token** tokens = ((Token* []) {
        makeToken("[", TOKEN_LBRACK),
        makeToken("1", TOKEN_NUMBER),
        makeToken(",", TOKEN_COMMA),
        makeToken("2", TOKEN_NUMBER),
        makeToken(",", TOKEN_COMMA),
        makeToken("]", TOKEN_RBRACK),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_ARRAY_LITERAL", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_ARRAY_LITERAL");
    ArrayLiteralNode* array = n->as.arrayLiteralNode;

    TestResult res = assertLiteralNode(testName, array->elements[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;
    return assertLiteralNode(testName, array->elements[1], LITERAL_NODE_INT, 2);
})

TEST(testArrayLiteralExpression_emptyArray, {
    Token** tokens = ((Token* []) {
        makeToken("[", TOKEN_LBRACK),
        makeToken("]", TOKEN_RBRACK),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_ARRAY_LITERAL", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_ARRAY_LITERAL");
    ArrayLiteralNode* array = n->as.arrayLiteralNode;
    ASSERT_EQ(0, array->size, "There should be no elements in the array");
})

TEST(testObjectLiteralExpression, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("key1", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("1", TOKEN_NUMBER),
        makeToken(",", TOKEN_COMMA),
        makeToken("key2", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("\"hello\"", TOKEN_STRING),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_OBJECT_LITERAL", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_OBJECT_LITERAL");
    ObjectLiteralNode* obj = n->as.objectLiteralNode;

    ASSERT_EQ(2, obj->size, "There should be 2 entries in the object");

    ObjectLiteralEntry* entry = obj->entries[0];
    ASSERT_EQ_STR("key1", entry->ident->as.identifierNode->name, "The first key should be key1");
    TestResult res = assertLiteralNode(testName, entry->value, LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

     entry = obj->entries[1];
    ASSERT_EQ_STR("key2", entry->ident->as.identifierNode->name, "The second key should be key2");
    return assertLiteralNode(testName, entry->value, LITERAL_NODE_STRING, "\"hello\"", 7);
})

TEST(testObjectLiteralExpression_trailingCommas, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("key1", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("1", TOKEN_NUMBER),
        makeToken(",", TOKEN_COMMA),
        makeToken("key2", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("\"hello\"", TOKEN_STRING),
        makeToken(",", TOKEN_COMMA),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_OBJECT_LITERAL", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_OBJECT_LITERAL");
    ObjectLiteralNode* obj = n->as.objectLiteralNode;

    ASSERT_EQ(2, obj->size, "There should be 2 entries in the object");
    ASSERT_EQ_STR("key1", obj->keys[0]->as.identifierNode->name, "The first key should be key1");
    ASSERT_EQ_STR("key2", obj->keys[1]->as.identifierNode->name, "The second key should be key2");

    ObjectLiteralEntry* entry = obj->entries[0];
    ASSERT_EQ_STR("key1", entry->ident->as.identifierNode->name, "The first key should be key1");
    TestResult res = assertLiteralNode(testName, entry->value, LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    entry = obj->entries[1];
    ASSERT_EQ_STR("key2", entry->ident->as.identifierNode->name, "The second key should be key2");
    return assertLiteralNode(testName, entry->value, LITERAL_NODE_STRING, "\"hello\"", 7);
})

TEST(testObjectLiteralExpression_emptyObject, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* nodes = parse(&p);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_OBJECT_LITERAL", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_OBJECT_LITERAL");
    ObjectLiteralNode* obj = n->as.objectLiteralNode;

    ASSERT_EQ(0, obj->size, "There should be 0 entries in the object");
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

TEST(testParseValDeclStatements, {
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
})

void runParserTests(Tester* tester) {
    tester->startSuite("Parser");
    tester->run(testParseIntLiteral);
    tester->run(testParseDoubleLiteral);
    tester->run(testParseBoolLiterals);
    tester->run(testParseStringLiteral);
    tester->run(testParseNilLiteral);
    tester->run(testParseIdentifier);
    tester->run(testUnaryExpression_minus);
    tester->run(testUnaryExpression_negate);
    tester->run(testBinaryExpression);
    tester->run(testBinaryExpression_precedences);
    tester->run(testArrayLiteralExpression);
    tester->run(testArrayLiteralExpression_emptyArray);
    tester->run(testArrayLiteralExpression_trailingCommas);
    tester->run(testObjectLiteralExpression);
    tester->run(testObjectLiteralExpression_trailingCommas);
    tester->run(testObjectLiteralExpression_emptyObject);
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
