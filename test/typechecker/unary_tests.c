#include "unary_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckUnaryNode_minusInt, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("-1");
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckUnaryNode_minusDouble, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("-1.43");
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckUnaryNode_errorMinusBoolean, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("-true", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 1);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckUnaryNode_errorMinusString, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("-'some string'", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 1);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* literal = (Node*) tc->nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, literal->type.type, "The type's value should be unset");
})

TEST(testTypecheckUnaryNode_bangBoolean, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("!true");
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckUnaryNode_errorBangInt, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("!3", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 1);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_INT, "Int");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckUnaryNode_errorBangString, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("!'some string'", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 1);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_BOOL, "Bool");

    Node* literal = (Node*) tc->nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, literal->type.type, "The type's value should be unset");
})

void runUnaryTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckUnaryNode_minusInt);
    tester->run(testTypecheckUnaryNode_minusDouble);
    tester->run(testTypecheckUnaryNode_errorMinusBoolean);
    tester->run(testTypecheckUnaryNode_errorMinusString);

    tester->run(testTypecheckUnaryNode_bangBoolean);
    tester->run(testTypecheckUnaryNode_errorBangInt);
    tester->run(testTypecheckUnaryNode_errorBangString);
}
