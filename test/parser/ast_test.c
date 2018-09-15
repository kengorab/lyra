#include <stdlib.h>
#include <string.h>
#include <parser/ast.h>

#include "parser/ast.h"
#include "ast_test.h"
#include "lexer/lexer.h"

static Token* makeToken(const char* contents, TokenType type);

TEST(testNewIntLiteralNode, {
    Token* t = makeToken("12", TOKEN_NUMBER);
    Node* n = newLiteralNode(t);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;
    ASSERT_EQ_STR("LITERAL_NODE_INT", literalNodeTypes[literalNode->type],
                  "The literal node should be of type LITERAL_NODE_INT");
    ASSERT_EQ(12, literalNode->iVal, "The literal node should contain iVal=12");
})

TEST(testNewIntLiteralNode_tokenContentsContainsDotOutsideScopeOfToken, {
    Token* t = makeToken("12", TOKEN_NUMBER);
    t->start = "12\nval b = 1.23";

    Node* n = newLiteralNode(t);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;
    ASSERT_EQ_STR("LITERAL_NODE_INT", literalNodeTypes[literalNode->type],
                  "The literal node should be of type LITERAL_NODE_INT");
    ASSERT_EQ(12, literalNode->iVal, "The literal node should contain iVal=12");
})

TEST(testNewDoubleLiteralNode, {
    Token* t = makeToken("1.23", TOKEN_NUMBER);
    Node* n = newLiteralNode(t);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;
    ASSERT_EQ_STR("LITERAL_NODE_DOUBLE", literalNodeTypes[literalNode->type],
                  "The literal node should be of type LITERAL_NODE_DOUBLE");
    ASSERT_EQ(1.23, literalNode->dVal, "The literal node should contain dVal=1.23");
})

TEST(testNewBoolLiteralNode_true, {
    Token* t = makeToken("true", TOKEN_TRUE);
    Node* n = newLiteralNode(t);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;
    ASSERT_EQ_STR("LITERAL_NODE_BOOL", literalNodeTypes[literalNode->type],
                  "The literal node should be of type LITERAL_NODE_BOOL");
    ASSERT_TRUE(literalNode->bVal, "The literal node should contain bVal=true");
})

TEST(testNewBoolLiteralNode_false, {
    Token* t = makeToken("false", TOKEN_FALSE);
    Node* n = newLiteralNode(t);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;
    ASSERT_EQ_STR("LITERAL_NODE_BOOL", literalNodeTypes[literalNode->type],
                  "The literal node should be of type LITERAL_NODE_BOOL");
    ASSERT_FALSE(literalNode->bVal, "The literal node should contain bVal=false");
})

TEST(testNewStringLiteralNode, {
    Token* t = makeToken("\"some string\"", TOKEN_STRING);
    Node* n = newLiteralNode(t);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;
    ASSERT_EQ_STR("LITERAL_NODE_STRING", literalNodeTypes[literalNode->type],
                  "The literal node should be of type LITERAL_NODE_STRING");
    ASSERT_EQ_STR("\"some string\"", literalNode->str.val, "The literal node should contain str.val=\"some string\"");
    ASSERT_EQ(13, literalNode->str.length, "The literal node should have str.length=13");
})

TEST(testNewStringLiteralNode_tokenContentsExtendsPastClosingQuote_strlengthIsCorrect, {
    Token* t = makeToken("\"some string\"", TOKEN_STRING);
    t->start = "\"some string\"\nval b = 1";
    Node* n = newLiteralNode(t);

    ASSERT_EQ_STR("NODE_TYPE_LITERAL", astNodeTypes[n->type], "The node should have type NODE_TYPE_LITERAL");
    LiteralNode* literalNode = n->as.literalNode;
    ASSERT_EQ_STR("LITERAL_NODE_STRING", literalNodeTypes[literalNode->type],
                  "The literal node should be of type LITERAL_NODE_STRING");
    ASSERT_EQ_STR("\"some string\"\nval b = 1", literalNode->str.val,
                  "The literal node should contain the full token value");
    ASSERT_EQ(13, literalNode->str.length, "The literal node should have str.length=13");
})

void runAstTests(Tester* tester) {
    tester->startSuite("AST");
    tester->run(testNewIntLiteralNode);
    tester->run(testNewIntLiteralNode_tokenContentsContainsDotOutsideScopeOfToken);
    tester->run(testNewDoubleLiteralNode);
    tester->run(testNewBoolLiteralNode_true);
    tester->run(testNewBoolLiteralNode_false);
    tester->run(testNewStringLiteralNode);
    tester->run(testNewStringLiteralNode_tokenContentsExtendsPastClosingQuote_strlengthIsCorrect);
}

static Token* makeToken(const char* contents, TokenType type) {
    Token* t = malloc(sizeof(Token));
    t->start = contents;
    t->length = (int) strlen(contents);
    t->type = type;
    t->col = 1;
    t->line = 1;
    return t;
}
