#include "unary_tests.h"

#include "literal_tests.h"

TEST(testCompileMinusUnaryExpr, {
    Typechecker* tc = PARSE_AND_GET_TC("-1.0", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(4, c.count, "There should be 4 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_NEGATE, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 4, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(1.0);
    return assertConstantPoolsEq(testName, 1, constantPool, c.constants.values);
})

TEST(testCompileMinusUnaryExpr_nested, {
    Typechecker* tc = PARSE_AND_GET_TC("-(-1.0)", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(5, c.count, "There should be 5 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_NEGATE, OP_NEGATE, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 4, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(1.0);
    return assertConstantPoolsEq(testName, 1, constantPool, c.constants.values);
})

void runUnaryCompilerTests(Tester* tester) {
    tester->run(testCompileMinusUnaryExpr);
    tester->run(testCompileMinusUnaryExpr_nested);
}
