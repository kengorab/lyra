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
    Token tokens[2];
    tokenizeStr("&+ |+", tokens);
    Token token;

    // Test token 1
    token = tokens[0];
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_ERROR", "Token should be of type TOKEN_ERROR");

    // Test token 2
    token = tokens[1];
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

TEST(testTokenizeIntegers, {
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

TEST(testTokenizeFloats, {
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

TEST(testTokenizeOperators, {
    Token tokens[12];
    tokenizeStr("+ - * / ! != < <= > >= && ||", tokens);
    Token token;

    char contents[3]; // Buffer for reading token contents into

    token = tokens[0];
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "+", "Token should contain \"+\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_PLUS", "Token should be of type TOKEN_PLUS");

    token = tokens[1];
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "-", "Token should contain \"-\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_MINUS", "Token should be of type TOKEN_MINUS");

    token = tokens[2];
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "*", "Token should contain \"*\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_STAR", "Token should be of type TOKEN_STAR");

    token = tokens[3];
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "/", "Token should contain \"/\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_SLASH", "Token should be of type TOKEN_SLASH");

    token = tokens[4];
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "!", "Token should contain \"!\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_BANG", "Token should be of type TOKEN_BANG");

    token = tokens[5];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 11, "Token should be on column 11");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "!=", "Token should contain \"!=\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_BANG_EQ", "Token should be of type TOKEN_BANG_EQ");

    token = tokens[6];
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "<", "Token should contain \"<\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_LT", "Token should be of type TOKEN_LT");

    token = tokens[7];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 16, "Token should be on column 16");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "<=", "Token should contain \"<=\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_LTE", "Token should be of type TOKEN_LTE");

    token = tokens[8];
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, ">", "Token should contain \">\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_GT", "Token should be of type TOKEN_GT");

    token = tokens[9];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 21, "Token should be on column 21");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, ">=", "Token should contain \">=\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_GTE", "Token should be of type TOKEN_GTE");

    token = tokens[10];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 24, "Token should be on column 24");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "&&", "Token should contain \"&&\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_AND", "Token should be of type TOKEN_AND");

    token = tokens[11];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 27, "Token should be on column 27");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "||", "Token should contain \"||\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_OR", "Token should be of type TOKEN_OR");
})

TEST(testTokenizeBrackets, {
    Token tokens[6];
    tokenizeStr("[] {} ()", tokens);

    char contents[2]; // Buffer for reading token contents into

    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "[", "Token should contain \"[\"");
    ASSERT_EQ_STR(tokenTypes[tokens[0].type], "TOKEN_LBRACK", "Token should be of type TOKEN_LBRACK");

    sprintf(contents, "%.*s", tokens[1].length, tokens[1].start);
    ASSERT_EQ_STR(contents, "]", "Token should contain \"]\"");
    ASSERT_EQ_STR(tokenTypes[tokens[1].type], "TOKEN_RBRACK", "Token should be of type TOKEN_RBRACK");

    sprintf(contents, "%.*s", tokens[2].length, tokens[2].start);
    ASSERT_EQ_STR(contents, "{", "Token should contain \"{\"");
    ASSERT_EQ_STR(tokenTypes[tokens[2].type], "TOKEN_LBRACE", "Token should be of type TOKEN_LBRACE");

    sprintf(contents, "%.*s", tokens[3].length, tokens[3].start);
    ASSERT_EQ_STR(contents, "}", "Token should contain \"}\"");
    ASSERT_EQ_STR(tokenTypes[tokens[3].type], "TOKEN_RBRACE", "Token should be of type TOKEN_RBRACE");

    sprintf(contents, "%.*s", tokens[4].length, tokens[4].start);
    ASSERT_EQ_STR(contents, "(", "Token should contain \"(\"");
    ASSERT_EQ_STR(tokenTypes[tokens[4].type], "TOKEN_LPAREN", "Token should be of type TOKEN_LPAREN");

    sprintf(contents, "%.*s", tokens[5].length, tokens[5].start);
    ASSERT_EQ_STR(contents, ")", "Token should contain \")\"");
    ASSERT_EQ_STR(tokenTypes[tokens[5].type], "TOKEN_RPAREN", "Token should be of type TOKEN_RPAREN");
})

TEST(testTokenizePunctuation, {
    Token tokens[3];
    tokenizeStr(". , :", tokens);

    char contents[2]; // Buffer for reading token contents into

    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, ".", "Token should contain \".\"");
    ASSERT_EQ_STR(tokenTypes[tokens[0].type], "TOKEN_DOT", "Token should be of type TOKEN_DOT");

    sprintf(contents, "%.*s", tokens[1].length, tokens[1].start);
    ASSERT_EQ_STR(contents, ",", "Token should contain \",\"");
    ASSERT_EQ_STR(tokenTypes[tokens[1].type], "TOKEN_COMMA", "Token should be of type TOKEN_COMMA");

    sprintf(contents, "%.*s", tokens[2].length, tokens[2].start);
    ASSERT_EQ_STR(contents, ":", "Token should contain \":\"");
    ASSERT_EQ_STR(tokenTypes[tokens[2].type], "TOKEN_COLON", "Token should be of type TOKEN_COLON");
})

TEST(testTokenizeValidString, {
    Token tokens[1];
    tokenizeStr("\"abc def\"", tokens);

    char contents[10]; // Buffer for reading token contents into

    ASSERT_EQ_INT(tokens[0].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[0].col, 1, "Token should be on column 1");
    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "\"abc def\"", "Token should contain \"abc def\"");
    ASSERT_EQ_STR(tokenTypes[tokens[0].type], "TOKEN_STRING", "Token should be of type TOKEN_STRING");
})

TEST(testTokenizeInvalidString_unterminatedByEOF, {
    Token tokens[1];
    tokenizeStr("\"abc def", tokens);

    char contents[20]; // Buffer for reading token contents into

    ASSERT_EQ_INT(tokens[0].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[0].col, 1, "Token should be on column 1");
    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "Unterminated string", "Token should be an 'Unterminated string' error");
    ASSERT_EQ_STR(tokenTypes[tokens[0].type], "TOKEN_ERROR", "Token should be of type TOKEN_ERROR");
})

TEST(testTokenizeInvalidString_unterminatedByNewline, {
    Token tokens[2];
    tokenizeStr("\"abc def\n\"ghi jkl", tokens);

    char contents[20]; // Buffer for reading token contents into

    ASSERT_EQ_INT(tokens[0].line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(tokens[0].col, 1, "Token should be on column 1");
    sprintf(contents, "%.*s", tokens[0].length, tokens[0].start);
    ASSERT_EQ_STR(contents, "Unterminated string", "Token should be an 'Unterminated string' error");
    ASSERT_EQ_STR(tokenTypes[tokens[0].type], "TOKEN_ERROR", "Token should be of type TOKEN_ERROR");

    ASSERT_EQ_INT(tokens[1].line, 2, "Token should be on line 2");
    ASSERT_EQ_INT(tokens[1].col, 1, "Token should be on column 1");
    sprintf(contents, "%.*s", tokens[1].length, tokens[1].start);
    ASSERT_EQ_STR(contents, "Unterminated string", "Token should be an 'Unterminated string' error");
    ASSERT_EQ_STR(tokenTypes[tokens[1].type], "TOKEN_ERROR", "Token should be of type TOKEN_ERROR");
})

TEST(testTokenizeKeywords, {
    Token tokens[9];
    tokenizeStr("val var type func if else true false nil", tokens);

    ASSERT_EQ_STR(tokenTypes[tokens[0].type], "TOKEN_VAL", "Token should be of type TOKEN_VAL");
    ASSERT_EQ_STR(tokenTypes[tokens[1].type], "TOKEN_VAR", "Token should be of type TOKEN_VAR");
    ASSERT_EQ_STR(tokenTypes[tokens[2].type], "TOKEN_TYPE", "Token should be of type TOKEN_TYPE");
    ASSERT_EQ_STR(tokenTypes[tokens[3].type], "TOKEN_FUNC", "Token should be of type TOKEN_FUNC");
    ASSERT_EQ_STR(tokenTypes[tokens[4].type], "TOKEN_IF", "Token should be of type TOKEN_IF");
    ASSERT_EQ_STR(tokenTypes[tokens[5].type], "TOKEN_ELSE", "Token should be of type TOKEN_ELSE");
    ASSERT_EQ_STR(tokenTypes[tokens[6].type], "TOKEN_TRUE", "Token should be of type TOKEN_TRUE");
    ASSERT_EQ_STR(tokenTypes[tokens[7].type], "TOKEN_FALSE", "Token should be of type TOKEN_FALSE");
    ASSERT_EQ_STR(tokenTypes[tokens[8].type], "TOKEN_NIL", "Token should be of type TOKEN_NIL");
})

TEST(testTokenizeIdentifiers, {
    Token tokens[9];
    tokenizeStr("hey helloWorld Point3D", tokens);
    Token token;

    char contents[11]; // Buffer for reading token contents into

    // Token 1
    token = tokens[0];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 1, "Token should be on column 1");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "hey", "Token should contain \"hey\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_IDENT", "Token should be of type TOKEN_IDENT");

    // Token 2
    token = tokens[1];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 5, "Token should be on column 5");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "helloWorld", "Token should contain \"helloWorld\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_IDENT", "Token should be of type TOKEN_IDENT");

    // Token 3
    token = tokens[2];
    ASSERT_EQ_INT(token.line, 1, "Token should be on line 1");
    ASSERT_EQ_INT(token.col, 16, "Token should be on column 16");
    sprintf(contents, "%.*s", token.length, token.start);
    ASSERT_EQ_STR(contents, "Point3D", "Token should contain \"Point3D\"");
    ASSERT_EQ_STR(tokenTypes[token.type], "TOKEN_IDENT", "Token should be of type TOKEN_IDENT");
})

void runLexerTests(Tester* tester) {
    tester->startSuite("Lexer");
    tester->run(testUnknownToken);
    tester->run(testLinesAndColumns);
    tester->run(testTokenizeIntegers);
    tester->run(testTokenizeFloats);
    tester->run(testTokenizeOperators);
    tester->run(testTokenizeBrackets);
    tester->run(testTokenizePunctuation);
    tester->run(testTokenizeValidString);
    tester->run(testTokenizeInvalidString_unterminatedByEOF);
    tester->run(testTokenizeInvalidString_unterminatedByNewline);
    tester->run(testTokenizeKeywords);
    tester->run(testTokenizeIdentifiers);
}
