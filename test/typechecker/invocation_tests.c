#include <typechecker/typechecker.h>
#include "invocation_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckInvocationNode_errorTargetNotInvokeable, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("(12 + 34)(1)", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 1, 10);
    ASSERT_EQ_STR("Cannot invoke type Int as function", err->custom.message,
                  "The error should have the expected message");
})

TEST(testTypecheckInvocationNode_errorInParam, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC(
        "{\n"
        "  func abc(a: Int) = a\n"
        "  abc(1 + true)\n"
        "}",
        1
    );
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_MISMATCH, err->kind, "The error should be a Type Mismatch error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 3, 9);
    ASSERT_TYPE_EQ(err->mismatch.actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->mismatch.numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckInvocationNode_errorIncorrectArity, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC(
        "{\n"
        "  func abc(a: Int, b: Int) = a + b\n"
        "  abc(1)\n"
        "}",
        1
    );
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 3, 6);
    ASSERT_EQ_STR("Expected 2 parameters, got 1", err->custom.message, "The error should have the expected message");
})

TEST(testTypecheckInvocationNode_errorParamTypeMismatch, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC(
        "{\n"
        "  func abc(a: Int, b: Int) = a\n"
        "  abc(1, '2')\n"
        "}",
        1
    );
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_MISMATCH, err->kind, "The error should be a Type Mismatch error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 3, 10);
    ASSERT_TYPE_EQ(err->mismatch.actualType, PRIMITIVE_TYPE_STRING, "String");
    ASSERT_EQ(1, err->mismatch.numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckInvocationNode_errorNamedParamNameMismatch, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC(
        "{\n"
        "  func abc(paramOne: Int, paramTwo: Int) = paramOne + paramTwo\n"
        "  abc(1, paramOne: 3)\n"
        "}",
        1
    );
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 3, 20);
    ASSERT_EQ_STR("Incorrect name for named parameter: expected paramTwo, got paramOne", err->custom.message,
                  "The error should have the expected message");
})

TEST(testTypecheckInvocationNode_typeIsFunctionReturnType, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC(
        "{\n"
        "  func abc(paramOne: Int, paramTwo: String) = paramOne + paramTwo\n"
        "  abc(1, paramTwo: '3')\n"
        "}",
        0
    );
    Node* n = (Node*) tc->nodes->values[0];
    ASSERT_TYPE_EQ(n->type, PRIMITIVE_TYPE_STRING, "String");
})

void runInvocationTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckInvocationNode_errorTargetNotInvokeable);
    tester->run(testTypecheckInvocationNode_errorInParam);
    tester->run(testTypecheckInvocationNode_errorIncorrectArity);
    tester->run(testTypecheckInvocationNode_errorParamTypeMismatch);
    tester->run(testTypecheckInvocationNode_errorNamedParamNameMismatch);

    tester->run(testTypecheckInvocationNode_typeIsFunctionReturnType);
}
