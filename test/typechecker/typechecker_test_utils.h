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

#define ASSERT_TYPE_LIST_OF(_type, typeEnum, typeStr) ({ \
    ASSERT_EQ(PRIMITIVE_TYPE_NONPRIMITIVE, _type->type, "The type's value should be PRIMITIVE_TYPE_NONPRIMITIVE"); \
    ASSERT_EQ_STR("List", _type->name, "The type's name should be List"); \
    ASSERT_EQ(1, _type->numTypeArgs, "The List should have 1 type argument"); \
    \
    ASSERT_EQ(typeEnum, _type->typeArgs[0]->type, "The list type argument's type value should be " #typeEnum); \
    ASSERT_EQ_STR(typeStr, _type->typeArgs[0]->name, "The list type argument's type name should be " typeStr); \
})

#define ASSERT_TOKEN_POSITION(token, lineNum, colNum) ({ \
    ASSERT_EQ(lineNum, token->line, "The error token should be on line " #lineNum); \
    ASSERT_EQ(colNum, token->col, "The error token should be on col " #colNum); \
})

#endif //CLYRA_TYPECHECKER_TEST_UTILS_H
