#ifndef CLYRA_TYPECHECKER_TEST_UTILS_H
#define CLYRA_TYPECHECKER_TEST_UTILS_H

#define PARSE_SINGLE_EXPR_NO_ERR(expr) ({ \
    Parser p = parseString(expr); \
    List* errorList = newList(); \
    List* nodes = parse(&p, &errorList); \
    Typechecker* tc = newTypechecker(nodes); \
    int numTypecheckErrors = typecheck(tc); \
    ASSERT_EQ(0, numTypecheckErrors, "There should be no typechecker errors"); \
    (Node*) nodes->values[0]; \
})

#define PARSE_SINGLE_EXPR_GET_TC(expr, numExpectedErrs) ({ \
    Parser p = parseString(expr); \
    List* errorList = newList(); \
    List* nodes = parse(&p, &errorList); \
    Typechecker* typechecker = newTypechecker(nodes); \
    int numTypecheckErrors = typecheck(typechecker); \
    ASSERT_EQ(numExpectedErrs, numTypecheckErrors, "There should be " #numExpectedErrs " typechecker errors"); \
    typechecker; \
})

#define ASSERT_TYPE_EQ(_type, typeEnum, typeStr) ({ \
    ASSERT_EQ(typeEnum, _type->type, "The type's value should be " #typeEnum); \
    ASSERT_EQ_STR(typeStr, _type->name, "The type's name should be " typeStr); \
})

#define ASSERT_TOKEN_POSITION(token, lineNum, colNum) ({ \
    ASSERT_EQ(lineNum, token->line, "The error token should be on line " #lineNum); \
    ASSERT_EQ(colNum, token->col, "The error token should be on col " #colNum); \
})

#endif //CLYRA_TYPECHECKER_TEST_UTILS_H
