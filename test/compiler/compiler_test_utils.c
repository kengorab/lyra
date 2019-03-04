#include <stdint.h>
#include <stdio.h>
#include <vm/value.h>

#include "compiler_test_utils.h"
#include "vm/common.h"

int numDigits(int digit) {
    int res = 0;
    if (digit < 0) {
        res++;
        digit = -digit;
    }

    if (digit < 10) return res + 1;
    if (digit < 100) return res + 2;
    return res + 3;
}

TestResult assertOpcodesEq(
    const char* testName,
    int size,
    const uint8_t* expected,
    const uint8_t* actual
) {
    for (int i = 0; i < size; ++i) {
        if (expected[i] != actual[i]) {
            if (i != 0 && expected[i - 1] == OP_CONSTANT) {
                int msgLen = 36 + numDigits(i) + 11 + numDigits(expected[i]) + 6 + numDigits(actual[i]) + 1;
                char msg[msgLen];
                sprintf(msg, "Constant index mismatch at position %d: expected %d, got %d", i, expected[i], actual[i]);
                FAIL(msg);
            } else {
                const char* expectedRepr = opcodes[expected[i]];
                const char* actualRepr = opcodes[actual[i]];
                size_t msgLen = 21 + (i / 10) + 11 + strlen(expectedRepr) + 6 + strlen(actualRepr) + 1;
                char msg[msgLen];

                sprintf(msg, "Mismatch at position %d: expected %s, got %s", i, expectedRepr, actualRepr);
                FAIL(msg);
            }
        }
    }

    PASS;
}

bool valuesEq(Value v1, Value v2) {
    return v1 == v2;
}

TestResult assertConstantPoolsEq(
    const char* testName,
    int size,
    Value* expected,
    Value* actual
) {
    for (int i = 0; i < size; ++i) {
        if (!valuesEq(expected[i], actual[i])) {
            int msgLen = 35 + numDigits(i) + 11 + numDigits(expected[i]) + 6 + numDigits(actual[i]) + 1;
            char msg[msgLen];
            sprintf(
                msg,
                "Constant pool mismatch at position %d: expected %f, got %f",
                i, expected[i], actual[i]
            );
            FAIL(msg);
        }
    }

    PASS;
}
