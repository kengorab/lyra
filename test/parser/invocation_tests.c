#include <string.h>
#include <parser/ast.h>

#include "invocation_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"

TEST(testInvocationExpression_noArgs, {
    Parser p = parseString("a()");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 0, "There should be no arguments to this invocation");
    return assertIdentNode(testName, invocation->target, "a");
})

TEST(testInvocationExpression_2ArgsUnnamed, {
    Parser p = parseString("a(1, \"a\")");
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 2, "There should be 2 arguments to this invocation");
    TestResult res = assertIdentNode(testName, invocation->target, "a");
    if (!res.pass) return res;

    res = assertLiteralNode(testName, invocation->arguments[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    res = assertLiteralNode(testName, invocation->arguments[1], LITERAL_NODE_STRING, "\"a\"", 3);
    if (!res.pass) return res;

    ASSERT_EQ_STR("", invocation->argNames[0], "The first arg name should be empty");
    ASSERT_EQ_STR("", invocation->argNames[1], "The second arg name should be empty");
})

TEST(testInvocationExpression_nestedExpr, {
    Parser p = parseString("a(1 + 2)");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->type], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 1, "There should be 1 argument to this invocation");
    TestResult res = assertIdentNode(testName, invocation->target, "a");
    if (!res.pass) return res;

    Node* arg = invocation->arguments[0];
    ASSERT_EQ_STR("NODE_TYPE_BINARY", astNodeTypes[arg->type], "The first arg should have type NODE_TYPE_BINARY");
    BinaryNode* binary = arg->as.binaryNode;

    res = assertLiteralNode(testName, binary->lExpr, LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    return assertLiteralNode(testName, binary->rExpr, LITERAL_NODE_INT, 2);
})

TEST(testInvocationExpression_nestedInvocations, {
    Parser p = parseString("a(b(\"b\"), \"a\")");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 2, "There should be 2 arguments to this invocation");
    TestResult res = assertIdentNode(testName, invocation->target, "a");
    if (!res.pass) return res;

    Node* arg = invocation->arguments[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[arg->nodeType],
                  "The first arg should have type NODE_TYPE_INVOCATION");
    ASSERT_TRUE(arg->as.invocationNode->numArgs == 1, "There should be 1 argument to this invocation");
    res = assertIdentNode(testName, arg->as.invocationNode->target, "b");
    if (!res.pass) return res;
    res = assertLiteralNode(testName, arg->as.invocationNode->arguments[0], LITERAL_NODE_STRING, "\"b\"", 3);

    return assertLiteralNode(testName, invocation->arguments[1], LITERAL_NODE_STRING, "\"a\"", 3);
})

TEST(testInvocationExpression_errorNoComma, {
    Parser p = parseString("abc(3 4)");

    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_COMMA", tokenTypes[error->expected[0]], "The expected token should be TOKEN_COMMA");
    ASSERT_EQ_STR("TOKEN_RPAREN", tokenTypes[error->expected[1]], "The expected token should be TOKEN_RPAREN");
    ASSERT_EQ_STR("TOKEN_NUMBER", tokenTypes[error->actual->type], "The actual token should be TOKEN_NUMBER");
})

TEST(testInvocationExpression_2ArgsNamed, {
    Parser p = parseString("a(a: 1, b: \"a\")");
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 2, "There should be 2 arguments to this invocation");
    TestResult res = assertIdentNode(testName, invocation->target, "a");
    if (!res.pass) return res;

    res = assertLiteralNode(testName, invocation->arguments[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    res = assertLiteralNode(testName, invocation->arguments[1], LITERAL_NODE_STRING, "\"a\"", 3);
    if (!res.pass) return res;

    ASSERT_EQ_STR("a", invocation->argNames[0], "The first named arg should be 'a'");
    ASSERT_EQ_STR("b", invocation->argNames[1], "The second named arg should be 'b'");
})

// This should _parse_ successfully, though it should fail to compile later on
TEST(testInvocationExpression_namedAndUnnamed, {
    Parser p = parseString("a(1, b: \"a\")");
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 2, "There should be 2 arguments to this invocation");
    TestResult res = assertIdentNode(testName, invocation->target, "a");
    if (!res.pass) return res;

    res = assertLiteralNode(testName, invocation->arguments[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    res = assertLiteralNode(testName, invocation->arguments[1], LITERAL_NODE_STRING, "\"a\"", 3);
    if (!res.pass) return res;

    ASSERT_EQ_STR("", invocation->argNames[0], "The first named arg should be empty");
    ASSERT_EQ_STR("b", invocation->argNames[1], "The second named arg should be 'b'");
})

void runInvocationTests(Tester* tester) {
    tester->run(testInvocationExpression_noArgs);
    tester->run(testInvocationExpression_2ArgsUnnamed);
    tester->run(testInvocationExpression_nestedExpr);
    tester->run(testInvocationExpression_nestedInvocations);
    tester->run(testInvocationExpression_errorNoComma);

    tester->run(testInvocationExpression_2ArgsNamed);
    tester->run(testInvocationExpression_namedAndUnnamed);
}
