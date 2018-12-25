#include <parser/ast.h>
#include "objectliteral_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckObjectLiteralNode, {
    Node* n = PARSE_SINGLE_EXPR_NO_ERR("{ field1: 1, fieldTwo: 'abc' }");
    Type* objType = n->type;
    ASSERT_EQ(PRIMITIVE_TYPE_NONPRIMITIVE, n->type->type, "The type's value should be PRIMITIVE_TYPE_NONPRIMITIVE");
    ASSERT_EQ_STR("Obj", objType->name, "The type's name should be Obj");

    const char* fieldNames[2];
    fieldNames[0] = "field1";
    fieldNames[1] = "fieldTwo";

    Type* fieldTypes[2];
    fieldTypes[0] = typeInt();
    fieldTypes[1] = typeString();

    for (int i = 0; i < objType->numTypeArgs; ++i) {
        ASSERT_TRUE(typeEq(fieldTypes[i], objType->typeArgs[i]), "The field types should match");
        ASSERT_EQ_STR(fieldNames[i], objType->typeArgNames[i], "The field names should match");
    }
})

TEST(testTypecheckObjectLiteralNode_nestedObjsAndArrays, {
    Node* n = PARSE_SINGLE_EXPR_NO_ERR("{ arrField: [1, 2, 3], objField: { a: true } }");
    Type* objType = n->type;
    ASSERT_EQ(PRIMITIVE_TYPE_NONPRIMITIVE, n->type->type, "The type's value should be PRIMITIVE_TYPE_NONPRIMITIVE");
    ASSERT_EQ_STR("Obj", objType->name, "The type's name should be Obj");

    const char* fieldName = objType->typeArgNames[0];
    ASSERT_EQ_STR("arrField", fieldName, "The field name should be 'arrField'");
    Type* fieldType = objType->typeArgs[0];
    ASSERT_TYPE_LIST_OF(fieldType, PRIMITIVE_TYPE_INT, "Int");

    fieldName = objType->typeArgNames[1];
    ASSERT_EQ_STR("objField", fieldName, "The field name should be 'objField'");
    fieldType = objType->typeArgs[1];
    ASSERT_EQ(PRIMITIVE_TYPE_NONPRIMITIVE, fieldType->type, "The type's value should be PRIMITIVE_TYPE_NONPRIMITIVE");
    ASSERT_EQ_STR("Obj", fieldType->name, "The type's name should be Obj");
    fieldName = fieldType->typeArgNames[0];
    ASSERT_EQ_STR("a", fieldName, "The field name should be 'a'");
    fieldType = fieldType->typeArgs[0];
    ASSERT_TYPE_EQ(fieldType, PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckObjectLiteralNode_errorWithinValue, {
    Typechecker* tc = PARSE_AND_GET_TC("{ a: true + false }", 1);
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_MISMATCH, err->kind, "The error should be a Type Mismatch error");
    ASSERT_TOKEN_POSITION(err->mismatch.token, 1, 11);
    ASSERT_TYPE_EQ(err->mismatch.actualType, PRIMITIVE_TYPE_BOOL, "Bool");
    ASSERT_EQ(2, err->mismatch.numExpected, "The error should include 2 expected types");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[0], PRIMITIVE_TYPE_INT, "Int");
    ASSERT_TYPE_EQ(err->mismatch.expectedTypes[1], PRIMITIVE_TYPE_DOUBLE, "Double");
})

void runObjectLiteralTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckObjectLiteralNode);
    tester->run(testTypecheckObjectLiteralNode_nestedObjsAndArrays);
    tester->run(testTypecheckObjectLiteralNode_errorWithinValue);
}
