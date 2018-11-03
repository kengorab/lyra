#include "literal_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckLiteralNode_int, {
    Parser p = parseString("1");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_INT, literal->type.type, "The type's value should be PRIMITIVE_TYPE_INT");
    ASSERT_EQ_STR("Int", literal->type.name, "The type's name should be Int");
})

TEST(testTypecheckLiteralNode_double, {
    Parser p = parseString("1.454");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_DOUBLE, literal->type.type, "The type's value should be PRIMITIVE_TYPE_DOUBLE");
    ASSERT_EQ_STR("Double", literal->type.name, "The type's name should be Double");
})

TEST(testTypecheckLiteralNode_bool, {
    Parser p = parseString("true");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_BOOL, literal->type.type, "The type's value should be PRIMITIVE_TYPE_BOOL");
    ASSERT_EQ_STR("Bool", literal->type.name, "The type's name should be Bool");
})

TEST(testTypecheckLiteralNode_string, {
    Parser p = parseString("'true'");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_STRING, literal->type.type, "The type's value should be PRIMITIVE_TYPE_STRING");
    ASSERT_EQ_STR("String", literal->type.name, "The type's name should be String");
})

TEST(testTypecheckLiteralNode_nil, {
    Parser p = parseString("nil");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_NIL, literal->type.type, "The type's value should be PRIMITIVE_TYPE_NIL");
    ASSERT_EQ_STR("Nil", literal->type.name, "The type's name should be Nil");
})

void runLiteralTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckLiteralNode_int);
    tester->run(testTypecheckLiteralNode_double);
    tester->run(testTypecheckLiteralNode_bool);
    tester->run(testTypecheckLiteralNode_string);
    tester->run(testTypecheckLiteralNode_nil);
}
