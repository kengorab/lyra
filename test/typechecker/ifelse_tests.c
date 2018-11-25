#include "ifelse_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckIfElseNode_errorCondNotBoolean, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("if (123) 'abc' else 'def'", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 5);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_INT, "Int");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckIfElseNode_thenAndElseBranchesSameType, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("if (true) 'abc' else 'def'");
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckIfElseNode_thenAndElseBranchesDifferentTypes, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("if (true) 'abc' else 123");
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, expr->type.type, "The type's value should be PRIMITIVE_TYPE_UNSET");
})

TEST(testTypecheckIfElseNode_noElseBranch, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("if (true) 'abc'");
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, expr->type.type, "The type's value should be PRIMITIVE_TYPE_UNSET");
})

void runIfElseTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckIfElseNode_errorCondNotBoolean);
    tester->run(testTypecheckIfElseNode_thenAndElseBranchesSameType);
    tester->run(testTypecheckIfElseNode_thenAndElseBranchesDifferentTypes);
    tester->run(testTypecheckIfElseNode_noElseBranch);
}
