#include "unary_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckUnaryNode_minusInt, {
    Parser p = parseString("-1");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_INT, literal->type.type, "The type's value should be PRIMITIVE_TYPE_INT");
    ASSERT_EQ_STR("Int", literal->type.name, "The type's name should be Int");
})

TEST(testTypecheckUnaryNode_minusDouble, {
    Parser p = parseString("-1.43");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_DOUBLE, literal->type.type, "The type's value should be PRIMITIVE_TYPE_DOUBLE");
    ASSERT_EQ_STR("Double", literal->type.name, "The type's name should be Double");
})

TEST(testTypecheckUnaryNode_errorMinusBoolean, {
    Parser p = parseString("-true");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(1, typecheckErrors->count, "There should be 1 typechecker error");
    TypecheckError* err = typecheckErrors->values[0];

    ASSERT_EQ(1, err->token->line, "The error token should be on line 1");
    ASSERT_EQ(1, err->token->col, "The error token should be on col 1");
    ASSERT_EQ(PRIMITIVE_TYPE_BOOL, err->actualType.type, "The actual type seen should be PRIMITIVE_TYPE_BOOL");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_EQ(PRIMITIVE_TYPE_INT, err->expectedTypes[0].type, "The first expected type should be PRIMITIVE_TYPE_INT");
    ASSERT_EQ(PRIMITIVE_TYPE_DOUBLE, err->expectedTypes[1].type, "The first expected type should be PRIMITIVE_TYPE_DOUBLE");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, literal->type.type, "The type's value should be unset");
})

TEST(testTypecheckUnaryNode_errorMinusString, {
    Parser p = parseString("-'some string'");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(1, typecheckErrors->count, "There should be 1 typechecker error");
    TypecheckError* err = typecheckErrors->values[0];

    ASSERT_EQ(1, err->token->line, "The error token should be on line 1");
    ASSERT_EQ(1, err->token->col, "The error token should be on col 1");
    ASSERT_EQ(PRIMITIVE_TYPE_STRING, err->actualType.type, "The actual type seen should be PRIMITIVE_TYPE_STRING");
    ASSERT_EQ(2, err->numExpected, "The error should include 2 expected types");
    ASSERT_EQ(PRIMITIVE_TYPE_INT, err->expectedTypes[0].type, "The first expected type should be PRIMITIVE_TYPE_INT");
    ASSERT_EQ(PRIMITIVE_TYPE_DOUBLE, err->expectedTypes[1].type, "The first expected type should be PRIMITIVE_TYPE_DOUBLE");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, literal->type.type, "The type's value should be unset");
})

TEST(testTypecheckUnaryNode_bangBoolean, {
    Parser p = parseString("!true");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_BOOL, literal->type.type, "The type's value should be PRIMITIVE_TYPE_BOOL");
    ASSERT_EQ_STR("Bool", literal->type.name, "The type's name should be Bool");
})

TEST(testTypecheckUnaryNode_errorBangInt, {
    Parser p = parseString("!3");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(1, typecheckErrors->count, "There should be 1 typechecker error");
    TypecheckError* err = typecheckErrors->values[0];

    ASSERT_EQ(1, err->token->line, "The error token should be on line 1");
    ASSERT_EQ(1, err->token->col, "The error token should be on col 1");
    ASSERT_EQ(PRIMITIVE_TYPE_INT, err->actualType.type, "The actual type seen should be PRIMITIVE_TYPE_INT");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_EQ(PRIMITIVE_TYPE_BOOL, err->expectedTypes[0].type, "The first expected type should be PRIMITIVE_TYPE_BOOL");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, literal->type.type, "The type's value should be unset");
})

TEST(testTypecheckUnaryNode_errorBangString, {
    Parser p = parseString("!'some string'");

    List* errorList = newList();
    List* nodes = parse(&p, &errorList);
    List* typecheckErrors = typecheck(nodes);
    ASSERT_EQ(1, typecheckErrors->count, "There should be 1 typechecker error");
    TypecheckError* err = typecheckErrors->values[0];

    ASSERT_EQ(1, err->token->line, "The error token should be on line 1");
    ASSERT_EQ(1, err->token->col, "The error token should be on col 1");
    ASSERT_EQ(PRIMITIVE_TYPE_STRING, err->actualType.type, "The actual type seen should be PRIMITIVE_TYPE_STRING");
    ASSERT_EQ(1, err->numExpected, "The error should include 1 expected type");
    ASSERT_EQ(PRIMITIVE_TYPE_BOOL, err->expectedTypes[0].type, "The first expected type should be PRIMITIVE_TYPE_BOOL");

    Node* literal = (Node*) nodes->values[0];
    ASSERT_EQ(PRIMITIVE_TYPE_UNSET, literal->type.type, "The type's value should be unset");
})

void runUnaryTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckUnaryNode_minusInt);
    tester->run(testTypecheckUnaryNode_minusDouble);
    tester->run(testTypecheckUnaryNode_errorMinusBoolean);
    tester->run(testTypecheckUnaryNode_errorMinusString);

    tester->run(testTypecheckUnaryNode_bangBoolean);
    tester->run(testTypecheckUnaryNode_errorBangInt);
    tester->run(testTypecheckUnaryNode_errorBangString);
}
