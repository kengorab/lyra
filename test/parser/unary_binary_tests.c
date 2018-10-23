#include <string.h>

#include "unary_binary_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"

TEST(testUnaryExpression_minus, {
    Parser p = parseString("-1.23");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_UNARY", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_UNARY");
    UnaryNode* unary = n->as.unaryNode;

    ASSERT_EQ_STR("-", tokenGetValue(unary->token), "The token should be a minus");
    return assertLiteralNode(testName, unary->expr, LITERAL_NODE_DOUBLE, 1.23);
})

TEST(testUnaryExpression_negate, {
    Parser p = parseString("!true");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_UNARY", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_UNARY");
    UnaryNode* unary = n->as.unaryNode;

    ASSERT_EQ_STR("!", tokenGetValue(unary->token), "The token should be a bang");
    return assertLiteralNode(testName, unary->expr, LITERAL_NODE_BOOL, true);
})

TEST(testBinaryExpression, {
    Parser p = parseString("1 + 2");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BINARY", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_BINARY");
    BinaryNode* binary = n->as.binaryNode;

    TestResult res = assertLiteralNode(testName, binary->lExpr, LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    ASSERT_EQ_STR("+", tokenGetValue(binary->token), "The token should be a +");
    return assertLiteralNode(testName, binary->rExpr, LITERAL_NODE_INT, 2);
})

TEST(testBinaryExpression_precedences, {
    // (((1 + (2 * 3)) >= 4) && (false == true))
    Parser p = parseString("1 + 2 * 3 >= 4 && false == true");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BINARY", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_BINARY");

    // The && expr
    BinaryNode* binary = n->as.binaryNode;
    ASSERT_EQ_STR("&&", tokenGetValue(binary->token), "The operator should be &&");

    // The == expr
    BinaryNode* rhs = binary->rExpr->as.binaryNode;
    ASSERT_EQ_STR("==", tokenGetValue(rhs->token), "The operator should be ==");
    TestResult res = assertLiteralNode(testName, rhs->lExpr, LITERAL_NODE_BOOL, false);
    if (!res.pass) return res;
    res = assertLiteralNode(testName, rhs->rExpr, LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;

    // The >= expr
    binary = binary->lExpr->as.binaryNode;
    ASSERT_EQ_STR(">=", tokenGetValue(binary->token), "The operator should be >=");
    res = assertLiteralNode(testName, binary->rExpr, LITERAL_NODE_INT, 4);
    if (!res.pass) return res;

    // The + expr
    binary = binary->lExpr->as.binaryNode;
    ASSERT_EQ_STR("+", tokenGetValue(binary->token), "The operator should be +");
    res = assertLiteralNode(testName, binary->lExpr, LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    // The * expr
    binary = binary->rExpr->as.binaryNode;
    ASSERT_EQ_STR("*", tokenGetValue(binary->token), "The operator should be *");
    res = assertLiteralNode(testName, binary->lExpr, LITERAL_NODE_INT, 2);
    if (!res.pass) return res;
    return assertLiteralNode(testName, binary->rExpr, LITERAL_NODE_INT, 3);
})

void runUnaryBinaryTests(Tester* tester) {
    tester->run(testUnaryExpression_minus);
    tester->run(testUnaryExpression_negate);

    tester->run(testBinaryExpression);
    tester->run(testBinaryExpression_precedences);
}
