#include "typedecl_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckTypeDeclNode_errorNotTopLevelScope, {
    Typechecker* tc = PARSE_AND_GET_TC(
        "{\n"
        "  type MyInt = Int\n"
        "}",
        1
    );

    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 2, 3);
    ASSERT_EQ_STR("Types cannot be declared outside of the top-level scope", err->custom.message,
                  "The error should have the expected message");
})

TEST(testTypecheckTypeDeclNode_aliasedBasicTypeInTypesMap, {
    Typechecker* tc = PARSE_AND_GET_TC("type MyInt = Int", 0);

    Type* type;
    get_TypesMap(tc->typesMap, "MyInt", &type);

    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckTypeDeclNode_aliasedTypeWithTypeArgsInTypesMap, {
    Typechecker* tc = PARSE_AND_GET_TC("type MyIntList = List[Int]", 0);

    Type* type;
    get_TypesMap(tc->typesMap, "MyIntList", &type);

    ASSERT_TYPE_LIST_OF(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckTypeDeclNode_usedInAnnotation, {
    Typechecker* tc = PARSE_AND_GET_TC(
        "type MyInt = Int\n"
        "val i: MyInt = 3",
        0
    );

    map_t scope;
    stack_peek(tc->scopes, &scope);

    Type* type;
    hashmap_get(scope, "i", (void**) &type);

    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

void runTypeDeclTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckTypeDeclNode_errorNotTopLevelScope);
    tester->run(testTypecheckTypeDeclNode_aliasedBasicTypeInTypesMap);
    tester->run(testTypecheckTypeDeclNode_aliasedTypeWithTypeArgsInTypesMap);
    tester->run(testTypecheckTypeDeclNode_usedInAnnotation);
}
