#ifndef CLYRA_TYPECHECKER_H
#define CLYRA_TYPECHECKER_H

#include <stdarg.h>

#include "parser/ast.h"
#include "common/list.h"
#include "common/depth_map.h"
#include "common/stack.h"

typedef struct {
    List* nodes;
    List* errors;
    NodeDepthMap* depthMap;
    lyra_stack_t* scopes;
} Typechecker;

Typechecker* newTypechecker(List* nodes);

typedef struct {
    Token* token;
    Type actualType;
    int numExpected;
    Type* expectedTypes;
} TypecheckError;

TypecheckError* newTypecheckError(Token* token, Type actualType, int numExpected, ...);

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
