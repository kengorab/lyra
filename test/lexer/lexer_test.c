#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "../test.h"

void tokenizeStr(char* str, Token* outTokens) {
    Lexer l = newLexer(str);

    int idx = 0;
    Token t = nextToken(&l);
    while (t.type != TOKEN_EOF) {
        outTokens[idx++] = t;
        t = nextToken(&l);
    }
}

TEST(testUnknownToken, {
    Token tokens[1];
    tokenizeStr("%", tokens);

    // Test token 1
    Token token = tokens[0];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 1, "Token should be on column 1");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_ERROR", "Token should be of type TOKEN_ERROR");
})

TEST(testLinesAndColumns, {
    Token tokens[3];
    tokenizeStr("1\n2\n3", tokens);
    Token token;

    // Test token 1
    token = tokens[0];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 1, "Token should be on column 1");

    // Test token 2
    token = tokens[1];
    ASSERT_EQ_INT(token.line, 2, "Token should be on line 2");
    ASSERT_EQ_INT(token.col, 1, "Token should be on column 1");

    // Test token 3
    token = tokens[2];
    ASSERT_EQ_INT(token.line, 3, "Token should be on line 3");
    ASSERT_EQ_INT(token.col, 1, "Token should be on column 1");
})

TEST(testTokenizingIntegers, {
    Token tokens[3];
    tokenizeStr("1 2 3", tokens);
    Token token;

    char contents[2]; // Buffer for reading token contents into

    // Test token 1
    token = tokens[0];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 1, "Token should be on column 1");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "1", "Token should contain \"1\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_NUMBER", "Token should be of type TOKEN_NUMBER");

    // Test token 2
    token = tokens[1];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 3, "Token should be on column 3");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "2", "Token should contain \"2\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_NUMBER", "Token should be of type TOKEN_NUMBER");

    // Test token 3
    token = tokens[2];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 5, "Token should be on column 5");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "3", "Token should contain \"3\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_NUMBER", "Token should be of type TOKEN_NUMBER");
})

TEST(testTokenizingFloats, {
    Token tokens[2];
    tokenizeStr("1.2 2.3", tokens);
    Token token;

    char contents[4]; // Buffer for reading token contents into

    // Test token 1
    token = tokens[0];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 1, "Token should be on column 1");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "1.2", "Token should contain \"1.2\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_NUMBER", "Token should be of type TOKEN_NUMBER");

    // Test token 2
    token = tokens[1];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 5, "Token should be on column 5");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "2.3", "Token should contain \"2.3\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_NUMBER", "Token should be of type TOKEN_NUMBER");
})

TEST(testTokenizingOperators, {
    Token tokens[12];
    tokenizeStr("1 + 2\n1 - 2\n1 * 2\n1 / 2", tokens);

    char contents[2]; // Buffer for reading token contents into

    // First line
    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "1", "Token should contain \"1\"");
    sprintf(contents, "%.*s", tokens[1].length, tokens[1].start);
    ASSERT_EQ_STR(contents, "+", "Token should contain \"+\"");
    sprintf(contents, "%.*s", tokens[2].length, tokens[2].start);
    ASSERT_EQ_STR(contents, "2", "Token should contain \"2\"");

    // Second line
    sprintf(contents, "%.*s", tokens[3].length, tokens[3].start);
    ASSERT_EQ_STR(contents, "1", "Token should contain \"1\"");
    sprintf(contents, "%.*s", tokens[4].length, tokens[4].start);
    ASSERT_EQ_STR(contents, "-", "Token should contain \"-\"");
    sprintf(contents, "%.*s", tokens[5].length, tokens[5].start);
    ASSERT_EQ_STR(contents, "2", "Token should contain \"2\"");

    // Third line
    sprintf(contents, "%.*s", tokens[6].length, tokens[6].start);
    ASSERT_EQ_STR(contents, "1", "Token should contain \"1\"");
    sprintf(contents, "%.*s", tokens[7].length, tokens[7].start);
    ASSERT_EQ_STR(contents, "*", "Token should contain \"*\"");
    sprintf(contents, "%.*s", tokens[8].length, tokens[8].start);
    ASSERT_EQ_STR(contents, "2", "Token should contain \"2\"");

    // Fourth line
    sprintf(contents, "%.*s", tokens[9].length, tokens[9].start);
    ASSERT_EQ_STR(contents, "1", "Token should contain \"1\"");
    sprintf(contents, "%.*s", tokens[10].length, tokens[10].start);
    ASSERT_EQ_STR(contents, "/", "Token should contain \"/\"");
    sprintf(contents, "%.*s", tokens[11].length, tokens[11].start);
    ASSERT_EQ_STR(contents, "2", "Token should contain \"2\"");
})

void runLexerTests(Tester* tester) {
    tester->startSuite("Lexer");
    tester->run(testUnknownToken);
    tester->run(testLinesAndColumns);
    tester->run(testTokenizingIntegers);
    tester->run(testTokenizingFloats);
    tester->run(testTokenizingOperators);
}
