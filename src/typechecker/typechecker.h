#ifndef CLYRA_TYPECHECKER_H
#define CLYRA_TYPECHECKER_H

#include <stdarg.h>

#include "parser/ast.h"
#include "common/enums.h"
#include "common/list.h"
#include "common/depth_map.h"
#include "common/stack.h"
#include "common/maps.h"
#include "typechecker/typedefs.h"

typedef struct {
    List* nodes;
    List* errors;
    NodeDepthMap* depthMap;
    lyra_stack_t* scopes;
    TypesMap* typesMap;
} Typechecker;

Typechecker* newTypechecker(List* nodes);

MAKE_ENUM(
    TypeErrorType,
    typeErrorTypes,

    TYPE_ERROR_MISMATCH,
    TYPE_ERROR_CUSTOM
)

typedef struct {
    TypeErrorType kind;

    union {
        struct {
            Token* token;
            Type* actualType;
            int numExpected;
            Type** expectedTypes;
        } mismatch;
        struct {
            Token* token;
            const char* message;
        } custom;
    };
} TypecheckError;

typedef TypecheckError* (* TypecheckFn)(Typechecker*, Node*);

typedef struct {
    AstNodeType nodeType;
    TypecheckFn fn;
} TypecheckRule;

/**
 * Returns the count of TypecheckErrors
 */
int typecheck(Typechecker* tc);

#endif //CLYRA_TYPECHECKER_H
