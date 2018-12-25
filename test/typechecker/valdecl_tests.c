#include "valdecl_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckValDeclNode_typeIsUnitAndRegistersBindingInScope, {
    Typechecker* tc = PARSE_AND_GET_TC("val a = 123", 0);

    Node* node = tc->nodes->values[0];
    ASSERT_TYPE_EQ(node->type, PRIMITIVE_TYPE_UNIT, "Unit");

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckValDeclNode_usingBindingInExpr, {
    Typechecker* tc = PARSE_AND_GET_TC(
        "val a = 12.3"
        "a + 4",
        0
    );

    Node* node = tc->nodes->values[1]; // a + 4
    ASSERT_TYPE_EQ(node->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckValDeclNode_typeAnnotationMatchingSimpleType, {
    Typechecker* tc = PARSE_AND_GET_TC("val a: Int = 123", 0);

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckValDeclNode_typeAnnotationMatchingTypeWithTypeArgs, {
    Typechecker* tc = PARSE_AND_GET_TC("val a: List[Int] = [123]", 0);

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_LIST_OF(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckValDeclNode_typeAnnotationTypeArgIsAny, {
    Typechecker* tc = PARSE_AND_GET_TC("val a: List[Any] = [123]", 0);

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_LIST_OF(type, PRIMITIVE_TYPE_ANY, "Any");
})

TEST(testTypecheckValDeclNode_typeAnnotationMissingRequiredTypeArg_treatedAsAny, {
    Typechecker* tc = PARSE_AND_GET_TC("val a: List = [123]", 0);

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_LIST_OF(type, PRIMITIVE_TYPE_ANY, "Any");
})

TEST(testTypecheckValDeclNode_errorTypeAnnotationNotMatchingType_bindingSavedWithAnnotatedType, {
    Typechecker* tc = PARSE_AND_GET_TC("val a: String = 123", 1);

    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_MISMATCH, err->kind, "The error should be a Type Mismatch error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 1, 17);
    ASSERT_TYPE_EQ(err->mismatch.actualType, PRIMITIVE_TYPE_INT, "Int");
    ASSERT_EQ(1, err->mismatch.numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[0], PRIMITIVE_TYPE_STRING, "String");

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckValDeclNode_errorNoAssignment, {
    Typechecker* tc = PARSE_AND_GET_TC("val a: String", 1);

    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 1, 1);
    ASSERT_EQ_STR("Missing required assignment for immutable variable a", err->custom.message,
                  "The error should have the expected message");
})

TEST(testTypecheckValDeclNode_errorRedeclaringVariable, {
    Typechecker* tc = PARSE_AND_GET_TC(
        "{\n"
        "  val a = 1\n"
        "  val a = 'abc'\n"
        "}",
        1
    );

    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 3, 3);
    ASSERT_EQ_STR("Variable a cannot be re-declared in this scope", err->custom.message,
                  "The error should have the expected message");
})

TEST(testTypecheckVarDeclNode_typeIsUnitAndRegistersBindingInScope, {
    Typechecker* tc = PARSE_AND_GET_TC("var a = 123", 0);

    Node* node = tc->nodes->values[0];
    ASSERT_TYPE_EQ(node->type, PRIMITIVE_TYPE_UNIT, "Unit");

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckVarDeclNode_typeAnnotationButNoAssignment, {
    Typechecker* tc = PARSE_AND_GET_TC("var a: Double", 0);

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckVarDeclNode_errorMissingRequiredTypeAnnotation, {
    Typechecker* tc = PARSE_AND_GET_TC("var a", 1);

    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 1, 1);
    ASSERT_EQ_STR("Missing required type annotation for mutable variable a", err->custom.message,
                  "The error should have the expected message");
})

void runValDeclTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckValDeclNode_typeIsUnitAndRegistersBindingInScope);
    tester->run(testTypecheckValDeclNode_usingBindingInExpr);
    tester->run(testTypecheckValDeclNode_typeAnnotationMatchingSimpleType);
    tester->run(testTypecheckValDeclNode_typeAnnotationMatchingTypeWithTypeArgs);
    tester->run(testTypecheckValDeclNode_typeAnnotationTypeArgIsAny);
    tester->run(testTypecheckValDeclNode_typeAnnotationMissingRequiredTypeArg_treatedAsAny);
    tester->run(testTypecheckValDeclNode_errorTypeAnnotationNotMatchingType_bindingSavedWithAnnotatedType);
    tester->run(testTypecheckValDeclNode_errorNoAssignment);
    tester->run(testTypecheckValDeclNode_errorRedeclaringVariable);

    tester->run(testTypecheckVarDeclNode_typeIsUnitAndRegistersBindingInScope);
    tester->run(testTypecheckVarDeclNode_typeAnnotationButNoAssignment);
    tester->run(testTypecheckVarDeclNode_errorMissingRequiredTypeAnnotation);
}
