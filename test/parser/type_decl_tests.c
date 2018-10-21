#include <string.h>
#include <parser/ast.h>

#include "type_decl_tests.h"
#include "parser/ast.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "test_utils.h"

TEST(testParseTypeDeclStatement_basicTypeNoTypeArgs, {
    Parser p = parseString("type Abc = Int");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Abc", typeDeclStmt->name->name, "The name of the type should be Abc");
    ASSERT_TRUE(typeDeclStmt->numArgs == 0, "There should be no type args for the type");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_BASIC_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_BASIC_TYPE");
    ASSERT_EQ_STR("Int", typeExpr->as.basicType.name->name, "The name of the basic type should be Int");
})

TEST(testParseTypeDeclStatement_basicTypeTypeArgsRight, {
    Parser p = parseString("type Abc = List[Int]");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Abc", typeDeclStmt->name->name, "The name of the type should be Abc");
    ASSERT_TRUE(typeDeclStmt->numArgs == 0, "There should be no type args for the type");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_BASIC_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_BASIC_TYPE");
    ASSERT_EQ_STR("List", typeExpr->as.basicType.name->name, "The name of the basic type should be List");
    ASSERT_EQ(1, typeExpr->numArgs, "There should be 1 type arg for the basic type");
    ASSERT_EQ_STR("Int", typeExpr->typeArgs[0]->as.basicType.name->name,
                  "The name of the basic type's type arg should be Int");
})

TEST(testParseTypeDeclStatement_basicTypeTypeArgsLeftAndRight, {
    Parser p = parseString("type Abc[T] = List[T]");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Abc", typeDeclStmt->name->name, "The name of the type should be Abc");
    ASSERT_TRUE(typeDeclStmt->numArgs == 1, "There should be 1 type arg for the type");
    ASSERT_EQ_STR("T", typeDeclStmt->typeArgs[0]->name, "The name of the basic type's type arg should be T");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_BASIC_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_BASIC_TYPE");
    ASSERT_EQ_STR("List", typeExpr->as.basicType.name->name, "The name of the basic type should be List");
    ASSERT_EQ(1, typeExpr->numArgs, "There should be 1 type arg for the basic type");
    ASSERT_EQ_STR("T", typeExpr->typeArgs[0]->as.basicType.name->name,
                  "The name of the basic type's type arg should be T");
})

TEST(testParseTypeDeclStatement_tupleType, {
    Parser p = parseString("type Point3D = [Int, Float, Double]");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Point3D", typeDeclStmt->name->name, "The name of the type should be Point3D");
    ASSERT_TRUE(typeDeclStmt->numArgs == 0, "There should be 0 type args for the type");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_TUPLE_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_TUPLE_TYPE");
    ASSERT_TRUE(typeExpr->numArgs == 3, "There should be 3 type args for the type");
    ASSERT_EQ_STR("Int", typeExpr->typeArgs[0]->as.basicType.name->name, "The first tuple type arg should be Int");
    ASSERT_EQ_STR("Float", typeExpr->typeArgs[1]->as.basicType.name->name, "The second tuple type arg should be Float");
    ASSERT_EQ_STR("Double", typeExpr->typeArgs[2]->as.basicType.name->name,
                  "The third tuple type arg should be Double");
})

TEST(testParseTypeDeclStatement_tupleTypeWithNestedTypes, {
    Parser p = parseString("type Something = [Int, Float, [Float, Double]]");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;
    ASSERT_EQ_STR("TYPE_EXPR_TUPLE_TYPE", typeExprTypes[typeDeclStmt->typeExpr->type],
                  "The type expression should have type TYPE_EXPR_TUPLE_TYPE");

    ASSERT_EQ_STR("Something", typeDeclStmt->name->name, "The name of the type should be Something");
    ASSERT_TRUE(typeDeclStmt->numArgs == 0, "There should be 0 type args for the type");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_TRUE(typeExpr->numArgs == 3, "There should be 3 type args for the type");
    ASSERT_EQ_STR("Int", typeExpr->typeArgs[0]->as.basicType.name->name, "The first tuple type arg should be Int");
    ASSERT_EQ_STR("Float", typeExpr->typeArgs[1]->as.basicType.name->name, "The second tuple type arg should be Float");

    TypeExpr* thirdArg = typeExpr->typeArgs[2];
    ASSERT_TRUE(thirdArg->numArgs == 2, "There should be 2 type args for the third tuple arg");
    ASSERT_EQ_STR("Float", thirdArg->typeArgs[0]->as.basicType.name->name, "The third tuple type arg should be Double");
    ASSERT_EQ_STR("Double", thirdArg->typeArgs[1]->as.basicType.name->name,
                  "The third tuple type arg should be Double");
})

TEST(testParseTypeDeclStatement_enumType, {
    Parser p = parseString("type Status = On | Off | Unset");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Status", typeDeclStmt->name->name, "The name of the type should be Something");
    ASSERT_TRUE(typeDeclStmt->numArgs == 0, "There should be 0 type args for the type");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_ENUM_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_ENUM_TYPE");
    ASSERT_TRUE(typeExpr->numArgs == 0, "There should be 0 type args for the type");

    ASSERT_TRUE(typeExpr->as.enumType.numOptions == 3, "There are 3 options for the enum type");
    ASSERT_EQ_STR("On", typeExpr->as.enumType.options[0]->as.basicType.name->name,
                  "The name of the first option should be On");
    ASSERT_EQ_STR("Off", typeExpr->as.enumType.options[1]->as.basicType.name->name,
                  "The name of the first option should be Off");
    ASSERT_EQ_STR("Unset", typeExpr->as.enumType.options[2]->as.basicType.name->name,
                  "The name of the first option should be Unset");
})

TEST(testParseTypeDeclStatement_enumTypeWithTypeArg, {
    Parser p = parseString("type Opt[T] = Some[T] | None");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Opt", typeDeclStmt->name->name, "The name of the type should be Opt");
    ASSERT_TRUE(typeDeclStmt->numArgs == 1, "There should be 1 type arg for the type");
    ASSERT_EQ_STR("T", typeDeclStmt->typeArgs[0]->name, "The name of the type should be T");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_ENUM_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_ENUM_TYPE");
    ASSERT_TRUE(typeExpr->numArgs == 0, "There should be 0 type args for the type");

    ASSERT_TRUE(typeExpr->as.enumType.numOptions == 2, "There are 2 options for the enum type");
    ASSERT_EQ_STR("Some", typeExpr->as.enumType.options[0]->as.basicType.name->name,
                  "The name of the first option should be Some");
    ASSERT_EQ_STR("T", typeExpr->as.enumType.options[0]->typeArgs[0]->as.basicType.name->name,
                  "The first option's type arg should be T");
    ASSERT_EQ_STR("None", typeExpr->as.enumType.options[1]->as.basicType.name->name,
                  "The name of the first option should be None");
})

TEST(testParseTypeDeclStatement_structType, {
    Parser p = parseString("type Person = { firstName: String, lastName: String }");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Person", typeDeclStmt->name->name, "The name of the type should be Person");
    ASSERT_TRUE(typeDeclStmt->numArgs == 0, "There should be 0 type args for the type");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_STRUCT_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_STRUCT_TYPE");
    ASSERT_TRUE(typeExpr->numArgs == 0, "There should be 0 type args for the type");

    ASSERT_EQ(2, typeExpr->as.structType.numFields, "There should be 2 fields for the Person type");
    ASSERT_EQ_STR("firstName", typeExpr->as.structType.keys[0]->as.identifierNode->name,
                  "The first key should be 'firstName'");
    ASSERT_EQ_STR("String", typeExpr->as.structType.fields[0]->as.basicType.name->name,
                  "The first field's type should be String");

    ASSERT_EQ_STR("lastName", typeExpr->as.structType.keys[1]->as.identifierNode->name,
                  "The first key should be 'lastName'");
    ASSERT_EQ_STR("String", typeExpr->as.structType.fields[1]->as.basicType.name->name,
                  "The second field's type should be String");
})

TEST(testParseTypeDeclStatement_structTypeWithNested, {
    Parser p = parseString("type Person = { name: { first: String, last: String }, age: Int }");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    ASSERT_EQ(1, nodes->count, "There should be 1 element in the list");

    Node* n = nodes->values[0];
    ASSERT_EQ_STR("NODE_TYPE_TYPE_DECL_STATEMENT", astNodeTypes[n->type],
                  "The node should have type NODE_TYPE_TYPE_DECL_STATEMENT");

    TypeDeclStmt* typeDeclStmt = n->as.typeDeclStmt;

    ASSERT_EQ_STR("Person", typeDeclStmt->name->name, "The name of the type should be Person");
    ASSERT_TRUE(typeDeclStmt->numArgs == 0, "There should be 0 type args for the type");

    TypeExpr* typeExpr = typeDeclStmt->typeExpr;
    ASSERT_EQ_STR("TYPE_EXPR_STRUCT_TYPE", typeExprTypes[typeExpr->type],
                  "The type expression should have type TYPE_EXPR_STRUCT_TYPE");
    ASSERT_TRUE(typeExpr->numArgs == 0, "There should be 0 type args for the type");

    ASSERT_EQ(2, typeExpr->as.structType.numFields, "There should be 2 fields for the Person type");
    ASSERT_EQ_STR("name", typeExpr->as.structType.keys[0]->as.identifierNode->name,
                  "The first key should be 'name'");
    TypeExpr* nameFieldTypeExpr = typeExpr->as.structType.fields[0];
    ASSERT_EQ_STR("TYPE_EXPR_STRUCT_TYPE", typeExprTypes[nameFieldTypeExpr->type],
                  "The type expression should have type TYPE_EXPR_STRUCT_TYPE");
    ASSERT_EQ_STR("first", nameFieldTypeExpr->as.structType.keys[0]->as.identifierNode->name,
                  "The first key should be 'first'");
    ASSERT_EQ_STR("String", nameFieldTypeExpr->as.structType.fields[0]->as.basicType.name->name,
                  "The first field's type should be String");
    ASSERT_EQ_STR("last", nameFieldTypeExpr->as.structType.keys[1]->as.identifierNode->name,
                  "The second key should be 'last'");
    ASSERT_EQ_STR("String", nameFieldTypeExpr->as.structType.fields[1]->as.basicType.name->name,
                  "The second field's type should be String");

    ASSERT_EQ_STR("age", typeExpr->as.structType.keys[1]->as.identifierNode->name,
                  "The second key should be 'age'");
    ASSERT_EQ_STR("Int", typeExpr->as.structType.fields[1]->as.basicType.name->name,
                  "The second field's type should be Int");
})

void runTypeDeclTests(Tester* tester) {
    tester->run(testParseTypeDeclStatement_basicTypeNoTypeArgs);
    tester->run(testParseTypeDeclStatement_basicTypeTypeArgsRight);
    tester->run(testParseTypeDeclStatement_basicTypeTypeArgsLeftAndRight);

    tester->run(testParseTypeDeclStatement_tupleType);
    tester->run(testParseTypeDeclStatement_tupleTypeWithNestedTypes);

    tester->run(testParseTypeDeclStatement_enumType);
    tester->run(testParseTypeDeclStatement_enumTypeWithTypeArg);

    tester->run(testParseTypeDeclStatement_structType);
    tester->run(testParseTypeDeclStatement_structTypeWithNested);
}
