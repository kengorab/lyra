#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "typechecker/typechecker_tests.h"
#include "literal_tests.h"
#include "unary_tests.h"
#include "binary_tests.h"
#include "ifelse_tests.h"
#include "block_tests.h"
#include "valdecl_tests.h"
#include "array_tests.h"
#include "funcdecl_tests.h"
#include "invocation_tests.h"
#include "objectliteral_tests.h"
#include "typedecl_tests.h"
#include "types_tests.h"

void runTypecheckerTests(Tester* tester) {
    tester->startSuite("Typechecker");

    tester->startSubsuite("Typechecker", "Types");
    runTypesTests(tester);

    tester->startSubsuite("Typechecker", "Literals");
    runLiteralTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Unary");
    runUnaryTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Binary");
    runBinaryTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "If-Else");
    runIfElseTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Block");
    runBlockTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Val-Decl");
    runValDeclTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Array");
    runArrayTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Func-Decl");
    runFuncDeclTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Invocation");
    runInvocationTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Objects");
    runObjectLiteralTypecheckerTests(tester);

    tester->startSubsuite("Typechecker", "Type-Decl");
    runTypeDeclTypecheckerTests(tester);
}
