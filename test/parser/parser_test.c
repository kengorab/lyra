#include "parser_test.h"

TEST(testPass, {
    PASS;
})

void runParserTests(Tester* tester) {
    tester->startSuite("Parser");
    tester->run(testPass);
}
