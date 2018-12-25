#include "funcdecl_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckFuncDeclNode_noParams, {
    Typechecker* tc = PARSE_AND_GET_TC("func returnOne() = 1", 0);
    Node* expr = (Node*) tc->nodes->values[0];
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_UNIT, "Unit");

    Type* type = GET_TYPE_FROM_SCOPE(tc, "returnOne");

    ASSERT_EQ(PRIMITIVE_TYPE_NONPRIMITIVE, type->type, "The type's value should be PRIMITIVE_TYPE_NONPRIMITIVE");
    ASSERT_EQ_STR("Function", type->name, "The type's name should be Function");
    ASSERT_EQ(1, type->numTypeArgs, "The Function should have 1 type argument (its return type)");
    ASSERT_TYPE_EQ(type->typeArgs[0], PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckFuncDeclNode_oneParam, {
    Typechecker* tc = PARSE_AND_GET_TC("func returnOne(a: String) = 1", 0);
    Node* expr = (Node*) tc->nodes->values[0];
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_UNIT, "Unit");

    Type* type = GET_TYPE_FROM_SCOPE(tc, "returnOne");

    ASSERT_EQ(PRIMITIVE_TYPE_NONPRIMITIVE, type->type, "The type's value should be PRIMITIVE_TYPE_NONPRIMITIVE");
    ASSERT_EQ_STR("Function", type->name, "The type's name should be List");
    ASSERT_EQ(2, type->numTypeArgs, "The Function should have 2 type arguments (its return type + param types)");
    ASSERT_TYPE_EQ(type->typeArgs[0], PRIMITIVE_TYPE_INT, "Int"); // Return type first
    ASSERT_TYPE_EQ(type->typeArgs[1], PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckFuncDeclNode_multiParams, {
    Typechecker* tc = PARSE_AND_GET_TC("func returnOne(a: String, b: Bool) = if (b) a else 'nope'", 0);
    Node* expr = (Node*) tc->nodes->values[0];
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_UNIT, "Unit");

    Type* type = GET_TYPE_FROM_SCOPE(tc, "returnOne");

    ASSERT_EQ(PRIMITIVE_TYPE_NONPRIMITIVE, type->type, "The type's value should be PRIMITIVE_TYPE_NONPRIMITIVE");
    ASSERT_EQ_STR("Function", type->name, "The type's name should be List");
    ASSERT_EQ(3, type->numTypeArgs, "The Function should have 3 type arguments (its return type + param types)");
    ASSERT_TYPE_EQ(type->typeArgs[0], PRIMITIVE_TYPE_STRING, "String"); // Return type first
    ASSERT_TYPE_EQ(type->typeArgs[1], PRIMITIVE_TYPE_STRING, "String");
    ASSERT_TYPE_EQ(type->typeArgs[2], PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckFuncDeclNode_errorDuplicateParameter, {
    Typechecker* tc = PARSE_AND_GET_TC("func returnOne(a: Int, a: Bool) = 1", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 1, 24);
    ASSERT_EQ_STR("Variable a cannot be re-declared in this scope", err->custom.message,
                  "The error should have the expected message");
})

TEST(testTypecheckFuncDeclNode_errorReturnTypeAnnotationMismatch, {
    Typechecker* tc = PARSE_AND_GET_TC("func returnOne(a: Int, b: Int): Bool = a + b", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_MISMATCH, err->kind, "The error should be a Type Mismatch error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 1, 42);
    ASSERT_TYPE_EQ(err->mismatch.actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(1, err->mismatch.numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckFuncDeclNode_errorWithinBody, {
    Typechecker* tc = PARSE_AND_GET_TC("func returnOne(a: Int, b: Bool) = a + b", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_MISMATCH, err->kind, "The error should be a Type Mismatch error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 1, 37);
    ASSERT_TYPE_EQ(err->mismatch.actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->mismatch.numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");
})

void runFuncDeclTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckFuncDeclNode_noParams);
    tester->run(testTypecheckFuncDeclNode_oneParam);
    tester->run(testTypecheckFuncDeclNode_multiParams);

    tester->run(testTypecheckFuncDeclNode_errorDuplicateParameter);
    tester->run(testTypecheckFuncDeclNode_errorReturnTypeAnnotationMismatch);
    tester->run(testTypecheckFuncDeclNode_errorWithinBody);
}
