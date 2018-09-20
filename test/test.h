#ifndef CLYRA_TEST_H
#define CLYRA_TEST_H

#include <stdbool.h>

typedef struct {
    const char* testName;
    bool pass;
    char* message;
} TestResult;

typedef TestResult (* TestFn)();

typedef void (* TestRunner)(TestFn);

typedef void (* SuiteStarter)(const char*);

typedef struct {
    const SuiteStarter startSuite;
    const TestRunner run;
} Tester;

#define TEST(name, block) \
    TestResult name() {\
        const char* testName = #name; \
        block\
        PASS;\
    }

#define PASS \
    do {\
        TestResult result = {.testName = testName};\
        result.pass = true;\
        return result;\
    } while (false)

#define FAIL(msg) \
    do {\
        TestResult result = {.testName = testName};\
        result.pass = false;\
        result.message = msg;\
        return result;\
    } while (false)

#define ASSERT_EQ(a, b, msg) if (a != b) FAIL(msg)

#define ASSERT_TRUE(v, msg) if (!(v)) FAIL(msg)

#define ASSERT_FALSE(v, msg) if ((v)) FAIL(msg)

#define ASSERT_EQ_STR(a, b, msg) if (strcmp(a, b) != 0) FAIL(msg)

#endif //CLYRA_TEST_H
