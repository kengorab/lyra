#include <string.h>

#include "func_decl_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "parser/utils.h"

TEST(testParseFuncDeclStatement_noParams, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken(")", TOKEN_RPAREN),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_FUNC_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_FUNC_DECL_STATEMENT");

    FuncDeclStmt* funcDeclStmt = n->as.funcDeclStmt;
    ASSERT_EQ_STR("myFunc", funcDeclStmt->name->name, "The ident should be abc");
    ASSERT_TRUE(funcDeclStmt->numParams == 0, "There should be no params for the function");

    return assertLiteralNode(testName, funcDeclStmt->body, LITERAL_NODE_INT, 123);
})

TEST(testParseFuncDeclStatement_with1Param, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken("a", TOKEN_IDENT),
        makeToken(")", TOKEN_RPAREN),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_FUNC_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_FUNC_DECL_STATEMENT");

    FuncDeclStmt* funcDeclStmt = n->as.funcDeclStmt;
    ASSERT_EQ_STR("myFunc", funcDeclStmt->name->name, "The ident should be abc");
    ASSERT_TRUE(funcDeclStmt->numParams == 1, "There should be 1 param for the function");
    TestResult res = assertIdentNode(testName, funcDeclStmt->params[0], "a");
    if (!res.pass) return res;

    return assertLiteralNode(testName, funcDeclStmt->body, LITERAL_NODE_INT, 123);
})

TEST(testParseFuncDeclStatement_with2Params_trailingComma, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken("a", TOKEN_IDENT),
        makeToken(",", TOKEN_COMMA),
        makeToken("b", TOKEN_IDENT),
        makeToken(",", TOKEN_COMMA),
        makeToken(")", TOKEN_RPAREN),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_FUNC_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_FUNC_DECL_STATEMENT");

    FuncDeclStmt* funcDeclStmt = n->as.funcDeclStmt;
    ASSERT_EQ_STR("myFunc", funcDeclStmt->name->name, "The ident should be abc");
    ASSERT_TRUE(funcDeclStmt->numParams == 2, "There should be 2 params for the function");
    TestResult res = assertIdentNode(testName, funcDeclStmt->params[0], "a");
    if (!res.pass) return res;
    res = assertIdentNode(testName, funcDeclStmt->params[1], "b");
    if (!res.pass) return res;

    return assertLiteralNode(testName, funcDeclStmt->body, LITERAL_NODE_INT, 123);
})

TEST(testParseFuncDeclStatement_blockAsBody, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken(")", TOKEN_RPAREN),
        makeToken("{", TOKEN_LBRACE),
        makeToken("123", TOKEN_NUMBER),
        makeToken("}", TOKEN_RBRACE),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_FUNC_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_FUNC_DECL_STATEMENT");

    FuncDeclStmt* funcDeclStmt = n->as.funcDeclStmt;
    ASSERT_EQ_STR("myFunc", funcDeclStmt->name->name, "The ident should be abc");
    ASSERT_TRUE(funcDeclStmt->numParams == 0, "There should be no params for the function");

    ASSERT_EQ_STR("NODE_TYPE_BLOCK", astNodeTypes[funcDeclStmt->body->type],
                  "The node should have type NODE_TYPE_BLOCK");
    ASSERT_TRUE(funcDeclStmt->body->as.blockNode->numExprs == 1, "There should be 1 expression in the function's body");

    return assertLiteralNode(testName, funcDeclStmt->body->as.blockNode->exprs[0], LITERAL_NODE_INT, 123);
})

TEST(testParseFuncDeclStatement_errorNoIdent, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("(", TOKEN_LPAREN),
        makeToken(")", TOKEN_RPAREN),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_IDENT", tokenTypes[error->expected[0]], "The expected token should be TOKEN_IDENT");
    ASSERT_EQ_STR("TOKEN_LPAREN", tokenTypes[error->actual->type], "The actual token should be TOKEN_LPAREN");
})

TEST(testParseFuncDeclStatement_errorNoLParen, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken(")", TOKEN_RPAREN),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_LPAREN", tokenTypes[error->expected[0]], "The expected token should be TOKEN_LPAREN");
    ASSERT_EQ_STR("TOKEN_RPAREN", tokenTypes[error->actual->type], "The actual token should be TOKEN_RPAREN");
})

TEST(testParseFuncDeclStatement_noParams_errorNoIdentOrRParen, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_IDENT", tokenTypes[error->expected[0]], "The expected token should be TOKEN_IDENT");
    ASSERT_EQ_STR("TOKEN_RPAREN", tokenTypes[error->expected[1]], "The expected token should be TOKEN_RPAREN");
    ASSERT_EQ_STR("TOKEN_EQ", tokenTypes[error->actual->type], "The actual token should be TOKEN_EQ");
})

TEST(testParseFuncDeclStatement_1Param_errorNoCommaOrRParen, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken("a", TOKEN_IDENT),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_COMMA", tokenTypes[error->expected[0]], "The expected token should be TOKEN_COMMA");
    ASSERT_EQ_STR("TOKEN_RPAREN", tokenTypes[error->expected[1]], "The expected token should be TOKEN_RPAREN");
    ASSERT_EQ_STR("TOKEN_EQ", tokenTypes[error->actual->type], "The actual token should be TOKEN_EQ");
})

TEST(testParseFuncDeclStatement_1ParamWithComma_errorNoRParen, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken("a", TOKEN_IDENT),
        makeToken(",", TOKEN_COMMA),
        makeToken("=", TOKEN_EQ),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_IDENT", tokenTypes[error->expected[0]], "The expected token should be TOKEN_IDENT");
    ASSERT_EQ_STR("TOKEN_RPAREN", tokenTypes[error->expected[1]], "The expected token should be TOKEN_RPAREN");
    ASSERT_EQ_STR("TOKEN_EQ", tokenTypes[error->actual->type], "The actual token should be TOKEN_EQ");
})

TEST(testParseFuncDeclStatement_errorNoEqOrLBrace, {
    Token** tokens = ((Token* []) {
        makeToken("func", TOKEN_FUNC),
        makeToken("myFunc", TOKEN_IDENT),
        makeToken("(", TOKEN_LPAREN),
        makeToken("a", TOKEN_IDENT),
        makeToken(")", TOKEN_RPAREN),
        makeToken("123", TOKEN_NUMBER),
        makeToken("", TOKEN_EOF),
    });

    Parser p = newParser(tokens);
    List* errorList = newList();
    parse(&p, &errorList);
    ASSERT_EQ(1, errorList->count, "There should be 1 error");

    ParseError* error = errorList->values[0];
    ASSERT_EQ_STR("TOKEN_LBRACE", tokenTypes[error->expected[0]], "The expected token should be TOKEN_LBRACE");
    ASSERT_EQ_STR("TOKEN_EQ", tokenTypes[error->expected[1]], "The expected token should be TOKEN_EQ");
    ASSERT_EQ_STR("TOKEN_NUMBER", tokenTypes[error->actual->type], "The actual token should be TOKEN_NUMBER");
})

void runFuncDeclTests(Tester* tester) {
    tester->run(testParseFuncDeclStatement_noParams);
    tester->run(testParseFuncDeclStatement_with1Param);
    tester->run(testParseFuncDeclStatement_with2Params_trailingComma);
    tester->run(testParseFuncDeclStatement_blockAsBody);
    tester->run(testParseFuncDeclStatement_errorNoIdent);
    tester->run(testParseFuncDeclStatement_errorNoLParen);
    tester->run(testParseFuncDeclStatement_noParams_errorNoIdentOrRParen);
    tester->run(testParseFuncDeclStatement_1Param_errorNoCommaOrRParen);
    tester->run(testParseFuncDeclStatement_1ParamWithComma_errorNoRParen);
    tester->run(testParseFuncDeclStatement_errorNoEqOrLBrace);
}
