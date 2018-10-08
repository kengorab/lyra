#include <string.h>

#include "literal_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "parser/utils.h"

TEST(testParseIntLiteral, {
    Token** tokens = ((Token* []) {
        makeToken("1", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_INT, 1);
})

TEST(testParseDoubleLiteral, {
    Token** tokens = ((Token* []) {
        makeToken("1.23", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
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
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
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
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
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
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_NIL);
})

TEST(testParseIdentifier, {
    Token** tokens = ((Token* []) {
        makeToken("abc", TOKEN_IDENT),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_IDENT", astNodeTypes[n->type], "The node type should be NODE_TYPE_IDENT");
})

void runLiteralTests(Tester* tester) {
    tester->run(testParseIntLiteral);
    tester->run(testParseDoubleLiteral);
    tester->run(testParseBoolLiterals);
    tester->run(testParseStringLiteral);
    tester->run(testParseNilLiteral);
    tester->run(testParseIdentifier);
}
