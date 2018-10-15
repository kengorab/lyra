#include <string.h>

#include "val_decl_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"

TEST(testParseValDeclStatement, {
    Parser p = parseString("val someValue = 123");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue", valDeclStmt->ident->name, "The ident should be someValue");
    ASSERT_FALSE(valDeclStmt->isMutable, "The binding should be declared as immutable");
    return assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_INT, 123);
})

TEST(testParseValDeclStatements, {
    Parser p = parseString("val someValue = 123"
                           "val someValue2 = \"hello!\"");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(2, nodes->count, "There should be 2 elements in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue", valDeclStmt->ident->name, "The ident should be someValue");
    ASSERT_FALSE(valDeclStmt->isMutable, "The binding should be declared as immutable");
    TestResult res = assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_INT, 123);
    if (!res.pass) return res;

    n = nodes->values[1];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue2", valDeclStmt->ident->name, "The ident should be someValue2");
    ASSERT_FALSE(valDeclStmt->isMutable, "The binding should be declared as immutable");
    return assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_STRING, "\"hello!\"", 8);
})

TEST(testParseValDeclStatement_errorNoIdent, {
    Parser p = parseString("val = 123");

    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_IDENT", tokenTypes[error->expected[0]], "The expected token should be TOKEN_IDENT");
    ASSERT_EQ_STR("TOKEN_EQ", tokenTypes[error->actual->type], "The actual token should be TOKEN_EQ");
})

TEST(testParseValDeclStatement_errorNoEq, {
    Parser p = parseString("val abc 123");

    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_EQ", tokenTypes[error->expected[0]], "The expected token should be TOKEN_EQ");
    ASSERT_EQ_STR("TOKEN_NUMBER", tokenTypes[error->actual->type], "The actual token should be TOKEN_NUMBER");
})

TEST(testParseValDeclStatement_errorNoExpr, {
    Parser p = parseString("val abc =");

    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("expression", error->expectedStr, "The expectation should say \"expression\"");
    ASSERT_EQ_STR("TOKEN_EOF", tokenTypes[error->actual->type], "The actual token should be TOKEN_EOF");
})

TEST(testParseVarDeclStatement_noAssignment, {
    Parser p = parseString("var someValue");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue", valDeclStmt->ident->name, "The ident should be someValue");
    ASSERT_TRUE(valDeclStmt->isMutable, "The binding should be declared as mutable");
    ASSERT_TRUE(valDeclStmt->assignment == NULL, "The binding shouldn't have an assignment");
})

TEST(testParseVarDeclStatement_withAssignment, {
    Parser p = parseString("var someValue = 123");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_VAL_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_VAL_DECL_STATEMENT");

    ValDeclStmt* valDeclStmt = n->as.valDeclStmt;
    ASSERT_EQ_STR("someValue", valDeclStmt->ident->name, "The ident should be someValue");
    ASSERT_TRUE(valDeclStmt->isMutable, "The binding should be declared as mutable");
    return assertLiteralNode(testName, valDeclStmt->assignment, LITERAL_NODE_INT, 123);
})

void runValDeclTests(Tester* tester) {
    tester->run(testParseValDeclStatement);
    tester->run(testParseValDeclStatements);
    tester->run(testParseValDeclStatement_errorNoIdent);
    tester->run(testParseValDeclStatement_errorNoEq);
    tester->run(testParseValDeclStatement_errorNoExpr);
    tester->run(testParseVarDeclStatement_noAssignment);
    tester->run(testParseVarDeclStatement_withAssignment);
}
