#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "typechecker/typechecker_tests.h"
#include "literal_tests.h"
#include "unary_tests.h"
#include "binary_tests.h"
#include "ifelse_tests.h"

void runTypecheckerTests(Tester* tester) {
    tester->startSuite("Typechecker");

    tester->startSubsuite("Typechecker", "Literals");
    runLiteralTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Unary");
    runUnaryTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Binary");
    runBinaryTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "If-Else");
    runIfElseTypecheckerTests(tester);
}
