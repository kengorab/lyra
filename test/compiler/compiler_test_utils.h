#ifndef CLYRA_COMPILER_TEST_UTILS_H
#define CLYRA_COMPILER_TEST_UTILS_H

#include "test_utils.h"
#include "vm/compiler.h"
#include "typechecker/typechecker_test_utils.h"

// Array literals cannot be constructed within the test macro blocks, so an additional layer of
// indirection is required
#define ARR(...) {__VA_ARGS__}

TestResult assertOpcodesEq(
    const char* testName,
    int size,
    const uint8_t* expected,
    const uint8_t* actual
);

TestResult assertConstantPoolsEq(
    const char* testName,
    int size,
    Value* expected,
    Value* actual
);

#endif //CLYRA_COMPILER_TEST_UTILS_H
