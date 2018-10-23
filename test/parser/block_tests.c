#include <string.h>

#include "common/strings.h"
#include "block_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"

TEST(testBlockExpression_singleExpr, {
    Parser p = parseString("{ \"expression!\" }");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_BLOCK");

    BlockNode* blockNode = n->as.blockNode;
    ASSERT_EQ(1, blockNode->numExprs, "There should be 1 expression-statement within the block node");

    return assertLiteralNode(testName, blockNode->exprs[0], LITERAL_NODE_STRING, "\"expression!\"", 13);
})

TEST(testBlockExpression_multipleExprs, {
    Parser p = parseString("{ key1 1 }");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_BLOCK");

    BlockNode* blockNode = n->as.blockNode;
    ASSERT_EQ(2, blockNode->numExprs, "There should be 2 expression-statements within the block node");

    TestResult res = assertIdentNode(testName, blockNode->exprs[0], "key1");
    if (!res.pass) return res;
    return assertLiteralNode(testName, blockNode->exprs[1], LITERAL_NODE_INT, 1);
})

TEST(testBlockExpression_statementsAndExpressions, {
    Parser p = parseString("{ val a = \"expression!\" a }");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[n->nodeType], "The node should have type NODE_TYPE_BLOCK");

    BlockNode* blockNode = n->as.blockNode;
    ASSERT_EQ(2, blockNode->numExprs, "There should be 2 expression-statements within the block node");

    Node* firstStmt = blockNode->exprs[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[firstStmt->nodeType],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = firstStmt->as.valDeclStmt;
    ASSERT_EQ_STR("a", valDeclStmt->ident->name, "The ident should be 'a'");
    ASSERT_FALSE(valDeclStmt->isMutable, "The binding should be declared as immutable");
    TestResult res = assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_STRING, "\"expression!\"", 13);
    if (!res.pass) return res;

    return assertIdentNode(testName, blockNode->exprs[1], "a");
})

TEST(testBlockExpression_assignmentToVal, {
    Parser p = parseString("val a = { 123 }");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->nodeType],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");
    ASSERT_EQ_STR("a", n->as.valDeclStmt->ident->name, "The ident should be 'a'");

    BlockNode* blockNode = n->as.valDeclStmt->assignment->as.blockNode;
    ASSERT_EQ(1, blockNode->numExprs, "There should be 1 expression-statement within the block node");

    Node* firstStmt = blockNode->exprs[0];
    return assertLiteralNode(testName, firstStmt, LITERAL_NODE_INT, 123);
})

void runBlockTests(Tester* tester) {
    tester->run(testBlockExpression_singleExpr);
    tester->run(testBlockExpression_multipleExprs);
    tester->run(testBlockExpression_statementsAndExpressions);
    tester->run(testBlockExpression_assignmentToVal);
}
