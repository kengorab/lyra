#ifndef CLYRA_TYPES_H
#define CLYRA_TYPES_H

#include <stdbool.h>
//#include "parser/ast.h"

#undef C
#define C(ENUM_VAL) ENUM_VAL,
#define PRIMITIVE_TYPE_TYPES \
    C(PRIMITIVE_TYPE_UNSET) \
    C(PRIMITIVE_TYPE_NONPRIMITIVE) \
    C(PRIMITIVE_TYPE_UNIT) \
    C(PRIMITIVE_TYPE_INT) \
    C(PRIMITIVE_TYPE_DOUBLE) \
    C(PRIMITIVE_TYPE_BOOL) \
    C(PRIMITIVE_TYPE_STRING) \
    C(PRIMITIVE_TYPE_NIL) \
    C(PRIMITIVE_TYPE_ANY)

typedef enum {
    PRIMITIVE_TYPE_TYPES
} PrimitiveType;

#undef C
#define C(ENUM_VAL) #ENUM_VAL,

// Ignore warning; initialized statically
const char* primitiveTypes[];

typedef struct Type Type;

struct Type {
    PrimitiveType type;
    const char* name;
    int numTypeArgs;
    Type** typeArgs;

    // Alright, this is a _little_ not great, but this will (for now) serve the purpose of
    // both keeping track of the types of parameter names in functions. This is as if the type
    // of `func abc(param1: Int): String` were Function[String, Int], an instance of tye type
    // Function[_ret, param1]. Again, not fantastic, but there shouldn't be any overhead with it
    // and it'll work for now.
    const char** typeArgNames;
};

Type* typeString();
Type* typeInt();
Type* typeDouble();
Type* typeBool();
Type* typeNil();
Type* typeAny();
Type* typeUnit();

Type* typeList(Type* typeArg);
Type* typeObj(const char* name, int numFields, Type** fieldTypes, const char** fieldNames);
Type* typeFunction(Type* returnType, int numArgs, Type** argTypes, const char** paramNames);

bool typeEq(Type* t1, Type* t2);

typedef struct TypeExpr TypeExpr; // Generates warning in ast.h for redefinition

Type* resolveType(TypeExpr* typeExpr);

#define NODE_IS_PRIMITIVE(n) (n->type->type != PRIMITIVE_TYPE_NONPRIMITIVE)
#define NODE_IS_INT(n) (n->type->type == PRIMITIVE_TYPE_INT)
#define NODE_IS_DOUBLE(n) (n->type->type == PRIMITIVE_TYPE_DOUBLE)
#define NODE_IS_NUMERIC(n) (NODE_IS_INT(n) || NODE_IS_DOUBLE(n))
#define NODE_IS_BOOL(n) (n->type->type == PRIMITIVE_TYPE_BOOL)
#define NODE_IS_STRING(n) (n->type->type == PRIMITIVE_TYPE_STRING)
#define NODE_IS_NIL(n) (n->type->type == PRIMITIVE_TYPE_NIL)
#define NODE_IS_FUNCTION(n) \
  (n->type->type == PRIMITIVE_TYPE_NONPRIMITIVE && strcmp("Function", n->type->name) == 0)

#endif //CLYRA_TYPES_H
