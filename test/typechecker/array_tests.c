#include "array_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckArrayNode_allSameType, {
    Node* array = PARSE_SINGLE_EXPR_NO_ERR("[1, 2, 3]");
    ASSERT_TYPE_LIST_OF(array->type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckArrayNode_notAllSameType, {
    Node* array = PARSE_SINGLE_EXPR_NO_ERR("[1, true, '3']");
    ASSERT_TYPE_LIST_OF(array->type, PRIMITIVE_TYPE_ANY, "Any");
})

TEST(testTypecheckArrayNode_errWithinArray, {
    Typechecker* tc = PARSE_AND_GET_TC("[1, true + false, 3]", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_MISMATCH, err->kind, "The error should be a Type Mismatch error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 1, 10);
    ASSERT_TYPE_EQ(err->mismatch.actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->mismatch.numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");
})

void runArrayTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckArrayNode_allSameType);
    tester->run(testTypecheckArrayNode_notAllSameType);
    tester->run(testTypecheckArrayNode_errWithinArray);
}
