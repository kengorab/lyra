#include "block_tests.h"
#include "typechecker/typechecker.h"
#include "test_utils.h"

TEST(testTypecheckBlockNode_singleExpr, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("{ 123 }");
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckBlockNode_multipleExprs, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("{ 123 'abc' }");
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckBlockNode_valDeclWithinBlock, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR("{"
                                          "  val a = '123'"
                                          "  a"
                                          "}");
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_STRING, "String");
})

TEST(testTypecheckBlockNode_valDeclWithinBlocks, {
    Node* expr = PARSE_SINGLE_EXPR_NO_ERR(
        "{ "
        "  val a = 123"
        "  {"
        "    val b = '123'"
        "    a"
        "  }"
        "}"
    );
    ASSERT_TYPE_EQ(expr->type, PRIMITIVE_TYPE_INT, "Int");
})

TEST(testTypecheckBlockNode_handlesErrorWithinBlock, {
    Typechecker* tc = PARSE_SINGLE_EXPR_GET_TC(
        "{\n"
        "  val a = 123\n"
        "  b\n"
        "}",
        1
    );
    TypecheckError* err = (TypecheckError*) tc->errors->values[0];
    ASSERT_EQ(TYPE_ERROR_CUSTOM, err->kind, "The error should be a Custom error");
    ASSERT_TOKEN_POSITION(err->custom.token, 3, 3);
    ASSERT_EQ_STR("Binding with name b not found", err->custom.message, "The error should have the expected message");
})

void runBlockTypecheckerTests(Tester* tester) {
    tester->run(testTypecheckBlockNode_singleExpr);
    tester->run(testTypecheckBlockNode_multipleExprs);
    tester->run(testTypecheckBlockNode_valDeclWithinBlock);
    tester->run(testTypecheckBlockNode_valDeclWithinBlocks);

    tester->run(testTypecheckBlockNode_handlesErrorWithinBlock);
}
