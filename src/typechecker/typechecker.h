#ifndef CLYRA_TYPECHECKER_H
#define CLYRA_TYPECHECKER_H

#include <stdarg.h>

#include "common/list.h"
#include "parser/ast.h"

typedef struct {
    Token* token;
    Type actualType;
    int numExpected;
    Type* expectedTypes;
} TypecheckError;

//TypecheckError* newTypecheckError(Token* token, const char* message);
TypecheckError* newTypecheckError(Token* token, Type actualType, int numExpected, ...);

typedef TypecheckError* (* TypecheckFn)(Node*);

typedef struct {
    AstNodeType nodeType;
    TypecheckFn fn;
} TypecheckRule;

/**
 * Returns a List of TypecheckErrors
 */
List* typecheck(List* nodes);

#endif //CLYRA_TYPECHECKER_H
