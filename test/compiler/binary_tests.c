#include "binary_tests.h"

TEST(testCompileAdditionBinaryExpr, {
    Typechecker* tc = PARSE_AND_GET_TC("1 + 1.0", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(6, c.count, "There should be 6 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_CONSTANT, 1, OP_ADD, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 4, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(1, 1.0);
    return assertConstantPoolsEq(testName, 2, constantPool, c.constants.values);
})

TEST(testCompileSubtractionBinaryExpr, {
    Typechecker* tc = PARSE_AND_GET_TC("1.0 - 1", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(6, c.count, "There should be 6 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_CONSTANT, 1, OP_SUB, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 4, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(1.0, 1);
    return assertConstantPoolsEq(testName, 2, constantPool, c.constants.values);
})

TEST(testCompileMultiplicationBinaryExpr, {
    Typechecker* tc = PARSE_AND_GET_TC("1 * 1.0", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(6, c.count, "There should be 6 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_CONSTANT, 1, OP_MUL, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 4, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(1, 1.0);
    return assertConstantPoolsEq(testName, 2, constantPool, c.constants.values);
})

TEST(testCompileDivisionBinaryExpr, {
    Typechecker* tc = PARSE_AND_GET_TC("1.0 / 1", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(6, c.count, "There should be 6 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_CONSTANT, 1, OP_DIV, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 4, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(1.0, 1);
    return assertConstantPoolsEq(testName, 2, constantPool, c.constants.values);
})

TEST(testCompileBinaryExpr_nested, {
    Typechecker* tc = PARSE_AND_GET_TC("((-6.24 + 24) * (16 / 8.0)) / 2 + 6.24", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(19, c.count, "There should be 19 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_NEGATE, OP_CONSTANT, 1, OP_ADD, OP_CONSTANT, 2, OP_CONSTANT, 3, OP_DIV, OP_MUL, OP_CONSTANT, 4, OP_DIV, OP_CONSTANT, 5, OP_ADD, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 4, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(6.24, 24, 16, 8.0, 2, 6.24);
    return assertConstantPoolsEq(testName, 2, constantPool, c.constants.values);
})

void runBinaryCompilerTests(Tester* tester) {
    tester->run(testCompileAdditionBinaryExpr);
    tester->run(testCompileSubtractionBinaryExpr);
    tester->run(testCompileMultiplicationBinaryExpr);
    tester->run(testCompileDivisionBinaryExpr);
    tester->run(testCompileBinaryExpr_nested);
}
