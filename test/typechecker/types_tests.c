#include <stdio.h>
#include <stdlib.h>

#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"
#include "types_tests.h"

static TypeExpr* parseTypeExpr(const char* expr);

static TypesMap* typesMap; // Constructed & initialized in the test entrypoint below

TEST(testResolveType_Int, {
    TypeExpr* typeExpr = parseTypeExpr("Int");
    Type* type = resolveType(typeExpr, typesMap);

    ASSERT_TYPE_EQ(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testResolveType_List, {
    TypeExpr* typeExpr = parseTypeExpr("List[Int]");
    Type* type = resolveType(typeExpr, typesMap);

    ASSERT_TYPE_LIST_OF(type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testResolveType_ListOfLists, {
    TypeExpr* typeExpr = parseTypeExpr("List[List[Int]]");
    Type* type = resolveType(typeExpr, typesMap);

    ASSERT_TYPE_LIST_OF(type, PRIMITIVE_TYPE_NONPRIMITIVE, "List");
    ASSERT_TYPE_LIST_OF(type->typeArgs[0], PRIMITIVE_TYPE_INT, "Int");
})

TEST(testResolveType_unknownType, {
    TypeExpr* typeExpr = parseTypeExpr("Unknown");
    Type* type = resolveType(typeExpr, typesMap);

    ASSERT_EQ(NULL, type, "The resolved type should be null");
})

TEST(testTypeEq_2EqPrimitives, {
    Type* _int = resolveType(parseTypeExpr("Int"), typesMap);
    ASSERT_TRUE(typeEq(_int, _int), "The two types should be equivalent");

    Type* _string = resolveType(parseTypeExpr("String"), typesMap);
    ASSERT_TRUE(typeEq(_string, _string), "The two types should be equivalent");

    Type* _bool = resolveType(parseTypeExpr("Bool"), typesMap);
    ASSERT_TRUE(typeEq(_bool, _bool), "The two types should be equivalent");

    Type* _double = resolveType(parseTypeExpr("Double"), typesMap);
    ASSERT_TRUE(typeEq(_double, _double), "The two types should be equivalent");
})

TEST(testTypeEq_AnyShouldBeEqualToAllTypes, {
    Type* _any = resolveType(parseTypeExpr("Any"), typesMap);

    Type* _int = resolveType(parseTypeExpr("Int"), typesMap);
    ASSERT_TRUE(typeEq(_any, _int), "The two types should be equivalent");

    Type* _string = resolveType(parseTypeExpr("String"), typesMap);
    ASSERT_TRUE(typeEq(_any, _string), "The two types should be equivalent");

    Type* _bool = resolveType(parseTypeExpr("Bool"), typesMap);
    ASSERT_TRUE(typeEq(_any, _bool), "The two types should be equivalent");

    Type* _double = resolveType(parseTypeExpr("Double"), typesMap);
    ASSERT_TRUE(typeEq(_any, _double), "The two types should be equivalent");

    Type* _listDouble = resolveType(parseTypeExpr("List[Double]"), typesMap);
    ASSERT_TRUE(typeEq(_any, _listDouble), "The two types should be equivalent");
})

TEST(testTypeEq_ListMatchingItself, {
    Type* _listInt = resolveType(parseTypeExpr("List[Int]"), typesMap);
    ASSERT_TRUE(typeEq(_listInt, _listInt), "The two types should be equivalent");

    Type* _listString = resolveType(parseTypeExpr("List[String]"), typesMap);
    ASSERT_TRUE(typeEq(_listString, _listString), "The two types should be equivalent");

    Type* _listBool = resolveType(parseTypeExpr("List[Bool]"), typesMap);
    ASSERT_TRUE(typeEq(_listBool, _listBool), "The two types should be equivalent");

    Type* _listDouble = resolveType(parseTypeExpr("List[Double]"), typesMap);
    ASSERT_TRUE(typeEq(_listDouble, _listDouble), "The two types should be equivalent");

    // ---------  Nested Lists  ---------

    Type* _listListInt = resolveType(parseTypeExpr("List[List[Int]]"), typesMap);
    ASSERT_TRUE(typeEq(_listListInt, _listListInt), "The two types should be equivalent");

    Type* _listListString = resolveType(parseTypeExpr("List[List[String]]"), typesMap);
    ASSERT_TRUE(typeEq(_listListString, _listListString), "The two types should be equivalent");

    Type* _listListBool = resolveType(parseTypeExpr("List[List[Bool]]"), typesMap);
    ASSERT_TRUE(typeEq(_listListBool, _listListBool), "The two types should be equivalent");

    Type* _listListDouble = resolveType(parseTypeExpr("List[List[Double]]"), typesMap);
    ASSERT_TRUE(typeEq(_listListDouble, _listListDouble), "The two types should be equivalent");
})

TEST(testTypeEq_ListMatchingListOfAny, {
    Type* _listAny = resolveType(parseTypeExpr("List[Any]"), typesMap);

    Type* _listInt = resolveType(parseTypeExpr("List[Int]"), typesMap);
    ASSERT_TRUE(typeEq(_listAny, _listInt), "The two types should be equivalent");

    Type* _listListInt = resolveType(parseTypeExpr("List[List[Int]]"), typesMap);
    ASSERT_TRUE(typeEq(_listAny, _listListInt), "The two types should be equivalent");
})

TEST(testTypeEq_matchingTypeWithParent, {
    Type* _number = newType("Number");
    Type* _int = newTypeWithParent("Int", _number);
    Type* _double = newTypeWithParent("Double", _number);

    ASSERT_TRUE(typeEq(_number, _int), "The two types should be equivalent");
    ASSERT_TRUE(typeEq(_number, _double), "The two types should be equivalent");
})

void runTypesTests(Tester* tester) {
    typesMap = newTypesMap();
    initTypesMap(typesMap);

    tester->run(testResolveType_Int);
    tester->run(testResolveType_List);
    tester->run(testResolveType_ListOfLists);
    tester->run(testResolveType_unknownType);

    tester->run(testTypeEq_2EqPrimitives);
    tester->run(testTypeEq_AnyShouldBeEqualToAllTypes);
    tester->run(testTypeEq_ListMatchingItself);
    tester->run(testTypeEq_ListMatchingListOfAny);

    tester->run(testTypeEq_matchingTypeWithParent);
}

static TypeExpr* parseTypeExpr(const char* expr) {
    char str[8 + strlen(expr)];
    snprintf(str, sizeof(str), "var x: %s", expr);

    Parser p = parseString(str);
    List* errorList = newList();
    List* nodes = parse(&p, &errorList);

    if (errorList->count != 0) {
        printf("Expected 0 errors: got %d\n", errorList->count);
        exit(1);
    }

    Node* node = (Node*) nodes->values[0];
    return node->as.valDeclStmt->typeAnnotation;
}
