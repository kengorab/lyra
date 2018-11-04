#include "binary_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

/* ------ Tests for + - * / operators ------ */

TEST(testTypecheckBinaryNode_plus_leftIsStringRightIsNot, {
    Node* binary = PARSE_SINGLE_EXPR_NO_ERR("'hello' + 1");
    ASSERT_TYPE_EQ(binary->type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckBinaryNode_plus_rightIsStringLeftIsNot, {
    Node* binary = PARSE_SINGLE_EXPR_NO_ERR("1 + 'hello'");
    ASSERT_TYPE_EQ(binary->type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckBinaryNode_arithmetic_leftIsIntRightIsInt, {
    Node* binaryPlus = PARSE_SINGLE_EXPR_NO_ERR("1 + 123");
    ASSERT_TYPE_EQ(binaryPlus->type, PRIMITIVE_TYPE_INT, "Int");

    Node* binaryMinus = PARSE_SINGLE_EXPR_NO_ERR("1 - 123");
    ASSERT_TYPE_EQ(binaryMinus->type, PRIMITIVE_TYPE_INT, "Int");

    Node* binaryMul = PARSE_SINGLE_EXPR_NO_ERR("1 * 123");
    ASSERT_TYPE_EQ(binaryMul->type, PRIMITIVE_TYPE_INT, "Int");

    Node* binaryDiv = PARSE_SINGLE_EXPR_NO_ERR("1 / 123");
    ASSERT_TYPE_EQ(binaryDiv->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckBinaryNode_arithmetic_leftIsIntRightIsDouble, {
    Node* binaryPlus = PARSE_SINGLE_EXPR_NO_ERR("1 + 12.3");
    ASSERT_TYPE_EQ(binaryPlus->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryMinus = PARSE_SINGLE_EXPR_NO_ERR("1 - 12.3");
    ASSERT_TYPE_EQ(binaryMinus->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryMul = PARSE_SINGLE_EXPR_NO_ERR("1 * 12.3");
    ASSERT_TYPE_EQ(binaryMul->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryDiv = PARSE_SINGLE_EXPR_NO_ERR("1 / 12.3");
    ASSERT_TYPE_EQ(binaryDiv->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckBinaryNode_arithmetic_leftIsDoubleRightIsInt, {
    Node* binaryPlus = PARSE_SINGLE_EXPR_NO_ERR("1.23 + 1");
    ASSERT_TYPE_EQ(binaryPlus->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryMinus = PARSE_SINGLE_EXPR_NO_ERR("1.23 - 1");
    ASSERT_TYPE_EQ(binaryMinus->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryMul = PARSE_SINGLE_EXPR_NO_ERR("1.23 * 1");
    ASSERT_TYPE_EQ(binaryMul->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryDiv = PARSE_SINGLE_EXPR_NO_ERR("1.23 / 1");
    ASSERT_TYPE_EQ(binaryDiv->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckBinaryNode_arithmetic_leftIsDoubleRightIsDouble, {
    Node* binaryPlus = PARSE_SINGLE_EXPR_NO_ERR("1.2 + 3.4");
    ASSERT_TYPE_EQ(binaryPlus->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryMinus = PARSE_SINGLE_EXPR_NO_ERR("1.2 - 3.4");
    ASSERT_TYPE_EQ(binaryMinus->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryMul = PARSE_SINGLE_EXPR_NO_ERR("1.2 * 3.4");
    ASSERT_TYPE_EQ(binaryMul->type, PRIMITIVE_TYPE_DOUBLE, "Double");

    Node* binaryDiv = PARSE_SINGLE_EXPR_NO_ERR("1.2 / 3.4");
    ASSERT_TYPE_EQ(binaryDiv->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckBinaryNode_arithmetic_errorNonNumeric, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("1 + true", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 3);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");

    tc = PARSE_SINGLE_EXPR_GET_TC("true - false", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 6);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");

    tc = PARSE_SINGLE_EXPR_GET_TC("false * 1.2", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 7);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");

    tc = PARSE_SINGLE_EXPR_GET_TC("1.2 / 'hello'", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 5);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");
})

/* ------ Tests for < <= > >= operators ------ */

TEST(testTypecheckBinaryNode_comparators_leftIsNumericRightIsNumeric, {
    Node* binaryLt = PARSE_SINGLE_EXPR_NO_ERR("1.2 < 3.4");
    ASSERT_TYPE_EQ(binaryLt->type, PRIMITIVE_TYPE_BOOL, "Bool");

    Node* binaryLte = PARSE_SINGLE_EXPR_NO_ERR("1.2 <= 3.4");
    ASSERT_TYPE_EQ(binaryLte->type, PRIMITIVE_TYPE_BOOL, "Bool");

    Node* binaryGt = PARSE_SINGLE_EXPR_NO_ERR("1.2 > 3.4");
    ASSERT_TYPE_EQ(binaryGt->type, PRIMITIVE_TYPE_BOOL, "Bool");

    Node* binaryGte = PARSE_SINGLE_EXPR_NO_ERR("1.2 >= 3.4");
    ASSERT_TYPE_EQ(binaryGte->type, PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckBinaryNode_comparators_errorNonNumeric, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("true < 3.4", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 6);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");

    tc = PARSE_SINGLE_EXPR_GET_TC("3.4 <= false", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 5);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");

    tc = PARSE_SINGLE_EXPR_GET_TC("3 > 'false'", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 3);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");

    tc = PARSE_SINGLE_EXPR_GET_TC("'abcd' >= 4", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 8);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");
})

/* ------ Tests for == != operators ------ */

TEST(testTypecheckBinaryNode_eqNeq, {
    Node* binaryEq = PARSE_SINGLE_EXPR_NO_ERR("'hello' == 4");
    ASSERT_TYPE_EQ(binaryEq->type, PRIMITIVE_TYPE_BOOL, "Bool");

    Node* binaryNeq = PARSE_SINGLE_EXPR_NO_ERR("[1, 2, 3] != 'abc'");
    ASSERT_TYPE_EQ(binaryNeq->type, PRIMITIVE_TYPE_BOOL, "Bool");
})

/* ------ Tests for && || operators ------ */

TEST(testTypecheckBinaryNode_and_leftIsBoolRightIsBool, {
    Node* binaryAnd = PARSE_SINGLE_EXPR_NO_ERR("true && (4 < 5)");
    ASSERT_TYPE_EQ(binaryAnd->type, PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckBinaryNode_or_leftIsBoolRightIsBool, {
    Node* binaryAnd = PARSE_SINGLE_EXPR_NO_ERR("false || (4 < 5)");
    ASSERT_TYPE_EQ(binaryAnd->type, PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckBinaryNode_andOr_errorNonBoolean, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("true && 3.4", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 6);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_DOUBLE, "Double");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_BOOL, "Bool");

    tc = PARSE_SINGLE_EXPR_GET_TC("'qwer' && false", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 8);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_BOOL, "Bool");

    tc = PARSE_SINGLE_EXPR_GET_TC("true || 3.4", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 6);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_DOUBLE, "Double");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_BOOL, "Bool");

    tc = PARSE_SINGLE_EXPR_GET_TC("'qwer' || false", 1);
    err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 8);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_BOOL, "Bool");
})

void runBinaryTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckBinaryNode_plus_leftIsStringRightIsNot);
    tester->run(testTypecheckBinaryNode_plus_rightIsStringLeftIsNot);
    tester->run(testTypecheckBinaryNode_arithmetic_leftIsIntRightIsInt);
    tester->run(testTypecheckBinaryNode_arithmetic_leftIsIntRightIsDouble);
    tester->run(testTypecheckBinaryNode_arithmetic_leftIsDoubleRightIsInt);
    tester->run(testTypecheckBinaryNode_arithmetic_leftIsDoubleRightIsDouble);
    tester->run(testTypecheckBinaryNode_arithmetic_errorNonNumeric);

    tester->run(testTypecheckBinaryNode_comparators_leftIsNumericRightIsNumeric);
    tester->run(testTypecheckBinaryNode_comparators_errorNonNumeric);

    tester->run(testTypecheckBinaryNode_eqNeq);

    tester->run(testTypecheckBinaryNode_and_leftIsBoolRightIsBool);
    tester->run(testTypecheckBinaryNode_or_leftIsBoolRightIsBool);
    tester->run(testTypecheckBinaryNode_andOr_errorNonBoolean);
}
