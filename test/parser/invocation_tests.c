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
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->type], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 0, "There should be no arguments to this invocation");
    return assertIdentNode(testName, invocation->target, "a");
})

TEST(testInvocationExpression_2Args, {
    Parser p = parseString("a(1, \"a\")");
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->type], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 2, "There should be 2 arguments to this invocation");
    TestResult res = assertIdentNode(testName, invocation->target, "a");
    if (!res.pass) return res;

    res = assertLiteralNode(testName, invocation->arguments[0], LITERAL_NODE_INT, 1);
    if (!res.pass) return res;

    return assertLiteralNode(testName, invocation->arguments[1], LITERAL_NODE_STRING, "\"a\"", 3);
})

TEST(testInvocationExpression_nestedInvocations, {
    Parser p = parseString("a(b(\"b\"), \"a\")");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[n->type], "The node should have type NODE_TYPE_INVOCATION");
    InvocationNode* invocation = n->as.invocationNode;

    ASSERT_TRUE(invocation->numArgs == 2, "There should be 2 arguments to this invocation");
    TestResult res = assertIdentNode(testName, invocation->target, "a");
    if (!res.pass) return res;

    Node* arg = invocation->arguments[0];
    ASSERT_EQ_STR("NODE_TYPE_INVOCATION", astNodeTypes[arg->type],
                  "The first arg should have type NODE_TYPE_INVOCATION");
    ASSERT_TRUE(arg->as.invocationNode->numArgs == 1, "There should be 1 argument to this invocation");
    res = assertIdentNode(testName, arg->as.invocationNode->target, "b");
    if (!res.pass) return res;
    res = assertLiteralNode(testName, arg->as.invocationNode->arguments[0], LITERAL_NODE_STRING, "\"b\"", 3);

    return assertLiteralNode(testName, invocation->arguments[1], LITERAL_NODE_STRING, "\"a\"", 3);
})

void runInvocationTests(Tester* tester) {
    tester->run(testInvocationExpression_noArgs);
    tester->run(testInvocationExpression_2Args);
    tester->run(testInvocationExpression_nestedInvocations);
}
