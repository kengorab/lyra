#include "literal_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckLiteralNode_int, {
    Node* lit = PARSE_SINGLE_EXPR_NO_ERR("1");
    ASSERT_TYPE_EQ(lit->type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckLiteralNode_double, {
    Node* lit = PARSE_SINGLE_EXPR_NO_ERR("1.454");
    ASSERT_TYPE_EQ(lit->type, PRIMITIVE_TYPE_DOUBLE, "Double");
})

TEST(testTypecheckLiteralNode_bool, {
    Node* lit = PARSE_SINGLE_EXPR_NO_ERR("true");
    ASSERT_TYPE_EQ(lit->type, PRIMITIVE_TYPE_BOOL, "Bool");
})

TEST(testTypecheckLiteralNode_string, {
    Node* lit = PARSE_SINGLE_EXPR_NO_ERR("'true'");
    ASSERT_TYPE_EQ(lit->type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckLiteralNode_nil, {
    Node* lit = PARSE_SINGLE_EXPR_NO_ERR("nil");
    ASSERT_TYPE_EQ(lit->type, PRIMITIVE_TYPE_NIL, "Nil");
})

void runLiteralTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckLiteralNode_int);
    tester->run(testTypecheckLiteralNode_double);
    tester->run(testTypecheckLiteralNode_bool);
    tester->run(testTypecheckLiteralNode_string);
    tester->run(testTypecheckLiteralNode_nil);
}
