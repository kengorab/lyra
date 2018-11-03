#ifndef CLYRA_TYPECHECKER_TEST_UTILS_H
#define CLYRA_TYPECHECKER_TEST_UTILS_H

#define PARSE_SINGLE_EXPR_NO_ERR(expr) ({ \
    Parser p = parseString(expr); \
    List* errorList = newList(); \
    List* nodes = parse(&p, &errorList); \
    List* typecheckErrors = typecheck(nodes); \
    ASSERT_EQ(0, typecheckErrors->count, "There should be no typechecker errors"); \
    (Node*) nodes->values[0]; \
})

#define PARSE_SINGLE_EXPR_GET_ERRS(expr, numExpectedErrs) ({ \
    Parser p = parseString(expr); \
    List* errorList = newList(); \
    List* nodes = parse(&p, &errorList); \
    List* typecheckErrors = typecheck(nodes); \
    ASSERT_EQ(numExpectedErrs, typecheckErrors->count, "There should be " #numExpectedErrs " typechecker errors"); \
    typecheckErrors; \
})

#define ASSERT_TYPE_EQ(_type, typeEnum, typeStr) ({ \
    ASSERT_EQ(typeEnum, _type.type, "The type's value should be " #typeEnum); \
    ASSERT_EQ_STR(typeStr, _type.name, "The type's name should be " typeStr); \
})

#define ASSERT_TOKEN_POSITION(token, lineNum, colNum) ({ \
    ASSERT_EQ(lineNum, token->line, "The error token should be on line " #lineNum); \
    ASSERT_EQ(colNum, token->col, "The error token should be on col " #colNum); \
})

#endif //CLYRA_TYPECHECKER_TEST_UTILS_H
