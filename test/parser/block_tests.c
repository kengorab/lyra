#include <string.h>

#include "block_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "parser/utils.h"

TEST(testBlockExpression_singleExpr, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("\"expression!\"", TOKEN_STRING),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[n->type], "The node should have type NODE_TYPE_BLOCK");

    BlockNode* blockNode = n->as.blockNode;
    ASSERT_EQ(1, blockNode->numExprs, "There should be 1 expression-statement within the block node");

    return assertLiteralNode(testName, blockNode->exprs[0], LITERAL_NODE_STRING, "\"expression!\"", 13);
})

TEST(testBlockExpression_multipleExprs, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("key1", TOKEN_IDENT),
        makeToken("1", TOKEN_NUMBER),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[n->type], "The node should have type NODE_TYPE_BLOCK");

    BlockNode* blockNode = n->as.blockNode;
    ASSERT_EQ(2, blockNode->numExprs, "There should be 2 expression-statements within the block node");

    TestResult res = assertIdentNode(testName, blockNode->exprs[0], "key1");
    if (!res.pass) return res;
    return assertLiteralNode(testName, blockNode->exprs[1], LITERAL_NODE_INT, 1);
})

TEST(testBlockExpression_statementsAndExpressions, {
    Token** tokens = ((Token* []) {
        makeToken("{", TOKEN_LBRACE),
        makeToken("val", TOKEN_VAL),
        makeToken("a", TOKEN_IDENT),
        makeToken("=", TOKEN_EQ),
        makeToken("\"expression!\"", TOKEN_STRING),
        makeToken("a", TOKEN_IDENT),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[n->type], "The node should have type NODE_TYPE_BLOCK");

    BlockNode* blockNode = n->as.blockNode;
    ASSERT_EQ(2, blockNode->numExprs, "There should be 2 expression-statements within the block node");

    Node* firstStmt = blockNode->exprs[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[firstStmt->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = firstStmt->as.valDeclStmt;
    ASSERT_EQ_STR("a", valDeclStmt->ident->name, "The ident should be 'a'");
    ASSERT_FALSE(valDeclStmt->isMutable, "The binding should be declared as immutable");
    TestResult res = assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_STRING, "\"expression!\"", 13);
    if (!res.pass) return res;

    return assertIdentNode(testName, blockNode->exprs[1], "a");
})

TEST(testBlockExpression_assignmentToVal, {
    Token** tokens = ((Token* []) {
        makeToken("val", TOKEN_VAL),
        makeToken("a", TOKEN_IDENT),
        makeToken("=", TOKEN_EQ),
        makeToken("{", TOKEN_LBRACE),
        makeToken("123", TOKEN_NUMBER),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
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
