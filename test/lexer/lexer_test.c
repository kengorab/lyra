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

TEST(testTokenizingIntegers, {
    Token tokens[3];
    tokenizeStr("1 2 3", tokens);

    char contents[2]; // Buffer for reading token contents into

    // Test token 1
    ASSERT_EQ_INT(tokens[0].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[0].col, 1, "Token should be on column 1");

    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "1", "Token should contain \"1\"");

    // Test token 2
    ASSERT_EQ_INT(tokens[1].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[1].col, 3, "Token should be on column 3");

    sprintf(contents, "%.*s", tokens[1].length, tokens[1].start);
    ASSERT_EQ_STR(contents, "2", "Token should contain \"2\"");

    // Test token 3
    ASSERT_EQ_INT(tokens[2].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[2].col, 5, "Token should be on column 5");

    sprintf(contents, "%.*s", tokens[2].length, tokens[2].start);
    ASSERT_EQ_STR(contents, "3", "Token should contain \"3\"");
})

TEST(testTokenizingFloats, {
    Token tokens[2];
    tokenizeStr("1.2 2.3", tokens);

    char contents[4]; // Buffer for reading token contents into

    // Test token 1
    ASSERT_EQ_INT(tokens[0].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[0].col, 1, "Token should be on column 1");

    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "1.2", "Token should contain \"1.2\"");

    // Test token 2
    ASSERT_EQ_INT(tokens[1].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[1].col, 5, "Token should be on column 5");

    sprintf(contents, "%.*s", tokens[1].length, tokens[1].start);
    ASSERT_EQ_STR(contents, "2.3", "Token should contain \"2.3\"");
})

TEST(testTokenizingOperators, {
    Token tokens[3];
    tokenizeStr("1 + 2", tokens);

    char contents[2]; // Buffer for reading token contents into
    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "1", "Token should contain \"1\"");
    sprintf(contents, "%.*s", tokens[1].length, tokens[1].start);
    ASSERT_EQ_STR(contents, "+", "Token should contain \"+\"");
    sprintf(contents, "%.*s", tokens[2].length, tokens[2].start);
    ASSERT_EQ_STR(contents, "2", "Token should contain \"2\"");
})

void runLexerTests(Tester* tester) {
    tester->startSuite("Lexer");
    tester->run(testTokenizingIntegers);
    tester->run(testTokenizingFloats);
    tester->run(testTokenizingOperators);
}
