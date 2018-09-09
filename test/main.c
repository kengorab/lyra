#include "lexer/lexer_test.h"
#include <stdio.h>

#define ANSI_RED  "\x1b[31m"
#define ANSI_GREEN  "\x1b[32m"
#define ANSI_ESC  "\x1b[0m"

static int numTests;
static int numPass;
static int numFail;

void startSuite(const char* suiteName) {
    printf("Running suite: %s\n", suiteName);
}

void testRunner(TestFn test) {
    numTests++;

    TestResult result = test();
    if (result.pass) {
        numPass++;
        printf("    Test %s... %s\n", result.testName, ANSI_GREEN "Pass" ANSI_ESC);
    } else {
        numFail++;
        printf("    Test %s... %s\n", result.testName, ANSI_RED "Fail" ANSI_ESC);
        printf("        Reason: %s\n", result.message);
    }
}

int main() {
    Tester t = {
        .startSuite = &startSuite,
        .run = &testRunner
    };

    runLexerTests(&t);

    printf(
        "Ran %d tests; %s%d successful%s, %s%d failure%s%s\n",
        numTests,
        ANSI_GREEN,
        numPass,
        ANSI_ESC,
        numFail == 0 ? ANSI_ESC : ANSI_RED,
        numFail,
        numFail == 0 ? "s" : "",
        ANSI_ESC
    );

    // Should only return a successful code if there are no failures
    return numFail;
}
