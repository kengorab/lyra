#include <string.h>

#include "array_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"

TEST(testArrayLiteralExpression, {
    Parser p = parseString("[1, 2]");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_ARRAY_LITERAL", astNodeTypes[n->nodeType],
                  "The node should have type NODE_TYPE_ARRAY_LITERAL");
    ArrayLiteralNode* array = n->as.arrayLiteralNode;

    TestResult res = assertLiteralNode(testName, array->elements[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;
    return assertLiteralNode(testName, array->elements[1], LITERAL_NODE_INT, 2);
})

TEST(testArrayLiteralExpression_trailingCommas, {
    Parser p = parseString("[1, 2,]");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_ARRAY_LITERAL", astNodeTypes[n->nodeType],
                  "The node should have type NODE_TYPE_ARRAY_LITERAL");
    ArrayLiteralNode* array = n->as.arrayLiteralNode;

    TestResult res = assertLiteralNode(testName, array->elements[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;
    return assertLiteralNode(testName, array->elements[1], LITERAL_NODE_INT, 2);
})

TEST(testArrayLiteralExpression_emptyArray, {
    Parser p = parseString("[]");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_ARRAY_LITERAL", astNodeTypes[n->nodeType],
                  "The node should have type NODE_TYPE_ARRAY_LITERAL");
    ArrayLiteralNode* array = n->as.arrayLiteralNode;
    ASSERT_EQ(0, array->size, "There should be no elements in the array");
})

TEST(testArrayLiteralExpression_errorNoCommaSeparator, {
    Parser p = parseString("[\"hello\"\"world\"]");

    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_COMMA", tokenTypes[error->expected[0]], "The expected token should be TOKEN_COMMA");
    ASSERT_EQ_STR("TOKEN_STRING", tokenTypes[error->actual->type], "The actual token should be TOKEN_STRING");
})

TEST(testArrayLiteralExpression_errorNoClosingBracket, {
    Parser p = parseString("[\"hello\", \"world\"");

    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_COMMA", tokenTypes[error->expected[0]], "The first expected token should be TOKEN_COMMA");
    ASSERT_EQ_STR("TOKEN_RBRACK", tokenTypes[error->expected[1]], "The second expected token should be TOKEN_RBRACK");
    ASSERT_EQ_STR("TOKEN_EOF", tokenTypes[error->actual->type], "The actual token should be TOKEN_EOF");
})

void runArrayTests(Tester* tester) {
    tester->run(testArrayLiteralExpression);
    tester->run(testArrayLiteralExpression_emptyArray);
    tester->run(testArrayLiteralExpression_trailingCommas);
    tester->run(testArrayLiteralExpression_errorNoCommaSeparator);
    tester->run(testArrayLiteralExpression_errorNoClosingBracket);
}
