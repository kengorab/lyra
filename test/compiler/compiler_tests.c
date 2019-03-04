#include "compiler_tests.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "compiler/compiler_tests.h"
#include "literal_tests.h"
#include "unary_tests.h"
#include "binary_tests.h"

void runCompilerTests(Tester* tester) {
    tester->startSuite("Compiler");

    tester->startSubsuite("Compiler", "Literals");
    runLiteralCompilerTests(tester);

    tester->startSubsuite("Compiler", "Unary");
    runUnaryCompilerTests(tester);

    tester->startSubsuite("Compiler", "Binary");
    runBinaryCompilerTests(tester);
}

