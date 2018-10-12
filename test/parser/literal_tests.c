#include <string.h>

#include "literal_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"

TEST(testParseIntLiteral, {
    Parser p = parseString("1");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_INT, 1);
})

TEST(testParseDoubleLiteral, {
    Parser p = parseString("1.23");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_DOUBLE, 1.23);
})

TEST(testParseBoolLiterals, {
    Parser p = parseString("true false");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(2, nodes->count, "There should be 2 elements in the list");

    TestResult res = assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;
    else return assertLiteralNode(testName, nodes->values[1], LITERAL_NODE_BOOL, false);
})

TEST(testParseStringLiteral, {
    Parser p = parseString("\"some string\"");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];

    return assertLiteralNode(testName, n, LITERAL_NODE_STRING, "\"some string\"", 13);
})

TEST(testParseNilLiteral, {
    Parser p = parseString("nil");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    return assertLiteralNode(testName, nodes->values[0], LITERAL_NODE_NIL);
})

TEST(testParseIdentifier, {
    Parser p = parseString("abc");

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
