#include <stdio.h>

#include "literal_tests.h"

TEST(testCompileIntLiteral, {
    Typechecker* tc = PARSE_AND_GET_TC("1", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(3, c.count, "There should be 3 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 3, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(1);
    return assertConstantPoolsEq(testName, 1, constantPool, c.constants.values);
})

TEST(testCompileDoubleLiteral, {
    Typechecker* tc = PARSE_AND_GET_TC("6.24", 0);
    Chunk c = compile(tc);

    ASSERT_EQ(3, c.count, "There should be 3 opcodes output");

    uint8_t ops[] = ARR(OP_CONSTANT, 0, OP_RETURN);
    TestResult res = assertOpcodesEq(testName, 3, ops, c.code);
    if (!res.pass) {
        return res;
    }

    Value constantPool[] = ARR(6.24);
    return assertConstantPoolsEq(testName, 1, constantPool, c.constants.values);
})

void runLiteralCompilerTests(Tester* tester) {
    tester->run(testCompileIntLiteral);
    tester->run(testCompileDoubleLiteral);
}
