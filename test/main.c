#include <stdio.h>

#include "common/list_test.h"
#include "lexer/lexer_test.h"
#include "parser/ast_test.h"
#include "parser/parser_test.h"

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
        printf("    %s %s\n", ANSI_GREEN "✓" ANSI_ESC, result.testName);
    } else {
        numFail++;
        printf("    %s %s%s\n", ANSI_RED "✘", result.testName, ANSI_ESC);
        printf("        Reason: %s\n", result.message);
    }
}

int main() {
    Tester t = {
        .startSuite = &startSuite,
        .run = &testRunner
    };

    runAstTests(&t);
    runListTests(&t);
    runLexerTests(&t);
    runParserTests(&t);

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
