#include <string.h>

#include "object_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "parser/utils.h"

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
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
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
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
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
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_OBJECT_LITERAL", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_OBJECT_LITERAL");
    ObjectLiteralNode* obj = n->as.objectLiteralNode;

    ASSERT_EQ(0, obj->size, "There should be 0 entries in the object");
})

TEST(testObjectLiteralExpression_errorNoIdent, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken(":", TOKEN_COLON),
        makeToken("1", TOKEN_NUMBER),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_IDENT", tokenTypes[error->expected[0]], "The expected token should be TOKEN_IDENT");
    ASSERT_EQ_STR("TOKEN_COLON", tokenTypes[error->actual->type], "The actual token should be TOKEN_COLON");
})

TEST(testObjectLiteralExpression_errorNoExpr, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("key1", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("expression", error->expectedStr, "The expectation should be \"expression\"");
    ASSERT_EQ_STR("TOKEN_RBRACE", tokenTypes[error->actual->type], "The actual token should be TOKEN_RBRACE");
})

TEST(testObjectLiteralExpression_errorNoComma, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("key1", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("1", TOKEN_NUMBER),
        makeToken("key2", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("2", TOKEN_NUMBER),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_COMMA", tokenTypes[error->expected[0]], "The expected token should be TOKEN_COMMA");
    ASSERT_EQ_STR("TOKEN_IDENT", tokenTypes[error->actual->type], "The actual token should be TOKEN_IDENT");
})

TEST(testObjectLiteralExpression_errorNoClosingBrace, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("key1", TOKEN_IDENT),
        makeToken(":", TOKEN_COLON),
        makeToken("1", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_COMMA", tokenTypes[error->expected[0]], "The first expected token should be TOKEN_COMMA");
    ASSERT_EQ_STR("TOKEN_RBRACE", tokenTypes[error->expected[1]], "The second expected token should be TOKEN_RBRACE");
    ASSERT_EQ_STR("TOKEN_EOF", tokenTypes[error->actual->type], "The actual token should be TOKEN_EOF");
})

void runObjectTests(Tester* tester) {
    tester->run(testObjectLiteralExpression);
    tester->run(testObjectLiteralExpression_trailingCommas);
    tester->run(testObjectLiteralExpression_emptyObject);
    tester->run(testObjectLiteralExpression_errorNoIdent);
    tester->run(testObjectLiteralExpression_errorNoExpr);
    tester->run(testObjectLiteralExpression_errorNoComma);
    tester->run(testObjectLiteralExpression_errorNoClosingBrace);
}
