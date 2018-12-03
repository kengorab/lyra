#include "valdecl_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckValDeclNode_typeIsUnitAndRegistersBindingInScope, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("val a = 123", 0);

    Node* node = tc->nodes->values[0];
    ASSERT_TYPE_EQ(node->type, PRIMITIVE_TYPE_UNIT, "Unit");

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckValDeclNode_usingBindingInExpr, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC(
        "val a = 12.3"
        "a + 4",
        0
    );

    Node* node = tc->nodes->values[1]; // a + 4
    ASSERT_TYPE_EQ(node->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckValDeclNode_typeAnnotationMatchingType, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("val a: Int = 123", 0);

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckValDeclNode_errorTypeAnnotationNotMatchingType_bindingSavedWithAnnotatedType, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("val a: String = 123", 1);

    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_TOKEN_POSITION(err->token, 1, 17);
    ASSERT_TYPE_EQ(err->actualType, PRIMITIVE_TYPE_INT, "Int");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_TYPE_EQ(err->expectedTypes[0], PRIMITIVE_TYPE_STRING, "String");

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckVarDeclNode_typeIsUnitAndRegistersBindingInScope, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("var a = 123", 0);

    Node* node = tc->nodes->values[0];
    ASSERT_TYPE_EQ(node->type, PRIMITIVE_TYPE_UNIT, "Unit");

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckVarDeclNode_typeAnnotationButNoAssignment, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC("var a: Double", 0);

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "a", (void**) &type);
    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

void runValDeclTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckValDeclNode_typeIsUnitAndRegistersBindingInScope);
    tester->run(testTypecheckValDeclNode_usingBindingInExpr);
    tester->run(testTypecheckValDeclNode_typeAnnotationMatchingType);
    tester->run(testTypecheckValDeclNode_errorTypeAnnotationNotMatchingType_bindingSavedWithAnnotatedType);

    tester->run(testTypecheckVarDeclNode_typeIsUnitAndRegistersBindingInScope);
    tester->run(testTypecheckVarDeclNode_typeAnnotationButNoAssignment);
}
