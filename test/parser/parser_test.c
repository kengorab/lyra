#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "literal_tests.h"
#include "unary_binary_tests.h"
#include "array_tests.h"
#include "object_tests.h"
#include "block_tests.h"
#include "ifelse_tests.h"
#include "val_decl_tests.h"
#include "func_decl_tests.h"
#include "invocation_tests.h"
#include "type_decl_tests.h"

void runParserTests(Tester* tester) {
    tester->startSuite("Parser");

    tester->startSubsuite("Parser", "Literals");
    runLiteralTests(tester);

    tester->startSubsuite("Parser", "Unary+Binary");
    runUnaryBinaryTests(tester);

    tester->startSubsuite("Parser", "Arrays");
    runArrayTests(tester);

    tester->startSubsuite("Parser", "Objects");
    runObjectTests(tester);

    tester->startSubsuite("Parser", "Blocks");
    runBlockTests(tester);

    tester->startSubsuite("Parser", "If-Else");
    runIfElseTests(tester);

    tester->startSubsuite("Parser", "Val+Var");
    runValDeclTests(tester);

    tester->startSubsuite("Parser", "Func-Declaration");
    runFuncDeclTests(tester);

    tester->startSubsuite("Parser", "Invocation");
    runInvocationTests(tester);

    tester->startSubsuite("Parser", "Type-Declaration");
    runTypeDeclTests(tester);
}
