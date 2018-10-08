#include <string.h>

#include "ifelse_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "parser/utils.h"

TEST(testParseIfElseStatement_noElse, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("true", TOKEN_TRUE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_IF_ELSE", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_IF_ELSE");

    IfElseNode* ifElseNode = n->as.ifElseNode;
    TestResult res = assertLiteralNode(testName, ifElseNode->conditionExpr, LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;
    res = assertLiteralNode(testName, ifElseNode->thenExpr, LITERAL_NODE_INT, 123);
    if (!res.pass) return res;
    ASSERT_TRUE(ifElseNode->elseExpr == NULL, "There should be no else expression");
})

TEST(testParseIfElseStatement_blockExpr_noElse, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("true", TOKEN_TRUE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("{", TOKEN_LBRACE),
        makeToken("123", TOKEN_NUMBER),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_IF_ELSE", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_IF_ELSE");

    IfElseNode* ifElseNode = n->as.ifElseNode;
    TestResult res = assertLiteralNode(testName, ifElseNode->conditionExpr, LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;

    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[ifElseNode->thenExpr->type],
                  "The node should have type NODE_TYPE_BLOCK");
    ASSERT_EQ(1, ifElseNode->thenExpr->as.blockNode->numExprs,
              "The then-expr block should have 1 expression-statement");
    return assertLiteralNode(testName, ifElseNode->thenExpr->as.blockNode->exprs[0], LITERAL_NODE_INT, 123);
})

TEST(testParseIfElseStatement_withElse, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("true", TOKEN_TRUE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("123", TOKEN_NUMBER),
        makeToken("else", TOKEN_ELSE),
        makeToken("456", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_IF_ELSE", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_IF_ELSE");

    IfElseNode* ifElseNode = n->as.ifElseNode;
    TestResult res = assertLiteralNode(testName, ifElseNode->conditionExpr, LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;
    res = assertLiteralNode(testName, ifElseNode->thenExpr, LITERAL_NODE_INT, 123);
    if (!res.pass) return res;
    return assertLiteralNode(testName, ifElseNode->elseExpr, LITERAL_NODE_INT, 456);
})

TEST(testParseIfElseStatement_nestedIfElseStmt, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("true", TOKEN_TRUE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("false", TOKEN_FALSE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("123", TOKEN_NUMBER),
        makeToken("else", TOKEN_ELSE),
        makeToken("456", TOKEN_NUMBER),
        makeToken("else", TOKEN_ELSE),
        makeToken("789", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_IF_ELSE", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_IF_ELSE");

    IfElseNode* ifElseNode = n->as.ifElseNode;
    TestResult res = assertLiteralNode(testName, ifElseNode->conditionExpr, LITERAL_NODE_BOOL, true);
    if (!res.pass) return res;

    ASSERT_EQ_STR("NODE_TYPE_IF_ELSE", astNodeTypes[ifElseNode->thenExpr->type],
                  "The node should have type NODE_TYPE_IF_ELSE");
    IfElseNode* nestedIfElseNode = n->as.ifElseNode->thenExpr->as.ifElseNode;
    res = assertLiteralNode(testName, nestedIfElseNode->conditionExpr, LITERAL_NODE_BOOL, false);
    if (!res.pass) return res;
    res = assertLiteralNode(testName, nestedIfElseNode->thenExpr, LITERAL_NODE_INT, 123);
    if (!res.pass) return res;
    res = assertLiteralNode(testName, nestedIfElseNode->elseExpr, LITERAL_NODE_INT, 456);
    if (!res.pass) return res;

    return assertLiteralNode(testName, ifElseNode->elseExpr, LITERAL_NODE_INT, 789);
})

TEST(testParseIfElseStatement_errorNoLParen, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("true", TOKEN_TRUE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("123", TOKEN_NUMBER),
        makeToken("else", TOKEN_ELSE),
        makeToken("456", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_LPAREN", tokenTypes[error->expected[0]], "The first expected token should be TOKEN_LPAREN");
    ASSERT_EQ_STR("TOKEN_TRUE", tokenTypes[error->actual->type], "The actual token should be TOKEN_TRUE");
})

TEST(testParseIfElseStatement_errorNoCondExpr, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken(")", TOKEN_RPAREN),
        makeToken("123", TOKEN_NUMBER),
        makeToken("else", TOKEN_ELSE),
        makeToken("456", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("expression", error->expectedStr, "The first expectation should be \"expression\"");
    ASSERT_EQ_STR("TOKEN_RPAREN", tokenTypes[error->actual->type], "The actual token should be TOKEN_RPAREN");
})

TEST(testParseIfElseStatement_errorNoRParen, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("true", TOKEN_TRUE),
        makeToken("123", TOKEN_NUMBER),
        makeToken("else", TOKEN_ELSE),
        makeToken("456", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_RPAREN", tokenTypes[error->expected[0]], "The first expected token should be TOKEN_RPAREN");
    ASSERT_EQ_STR("TOKEN_NUMBER", tokenTypes[error->actual->type], "The actual token should be TOKEN_NUMBER");
})

TEST(testParseIfElseStatement_errorNoThenExpr, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("true", TOKEN_TRUE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("else", TOKEN_ELSE),
        makeToken("456", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("expression", error->expectedStr, "The first expectation should be \"expression\"");
    ASSERT_EQ_STR("TOKEN_ELSE", tokenTypes[error->actual->type], "The actual token should be TOKEN_ELSE");
})

TEST(testParseIfElseStatement_errorNoElseExpr, {
    Token** tokens = ((Token* []) {
        makeToken("if", TOKEN_IF),
        makeToken("(", TOKEN_LPAREN),
        makeToken("true", TOKEN_TRUE),
        makeToken(")", TOKEN_RPAREN),
        makeToken("123", TOKEN_NUMBER),
        makeToken("else", TOKEN_ELSE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("expression", error->expectedStr, "The first expectation should be \"expression\"");
    ASSERT_EQ_STR("TOKEN_EOF", tokenTypes[error->actual->type], "The actual token should be TOKEN_EOF");
})

void runIfElseTests(Tester* tester) {
    tester->run(testParseIfElseStatement_noElse);
    tester->run(testParseIfElseStatement_blockExpr_noElse);
    tester->run(testParseIfElseStatement_withElse);
    tester->run(testParseIfElseStatement_nestedIfElseStmt);
    tester->run(testParseIfElseStatement_errorNoLParen);
    tester->run(testParseIfElseStatement_errorNoCondExpr);
    tester->run(testParseIfElseStatement_errorNoRParen);
    tester->run(testParseIfElseStatement_errorNoThenExpr);
    tester->run(testParseIfElseStatement_errorNoElseExpr);
}
