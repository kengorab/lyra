#ifndef CLYRA_TYPECHECKER_H
#define CLYRA_TYPECHECKER_H

#include "common/list.h"
#include "parser/ast.h"

typedef void (* TypecheckFn)(Node*);

typedef struct {
    AstNodeType nodeType;
    TypecheckFn fn;
} TypecheckRule;

void typecheck(List* nodes);

#endif //CLYRA_TYPECHECKER_H
