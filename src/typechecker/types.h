#ifndef CLYRA_TYPES_H
#define CLYRA_TYPES_H

#include <stdbool.h>
#include "typedefs.h"
#include "common/enums.h"

MAKE_ENUM(
    PrimitiveType,
    primitiveTypes,

    PRIMITIVE_TYPE_UNSET,
    PRIMITIVE_TYPE_NONPRIMITIVE,
    PRIMITIVE_TYPE_UNIT,
    PRIMITIVE_TYPE_INT,
    PRIMITIVE_TYPE_DOUBLE,
    PRIMITIVE_TYPE_BOOL,
    PRIMITIVE_TYPE_STRING,
    PRIMITIVE_TYPE_NIL,
    PRIMITIVE_TYPE_ANY
)

typedef struct Type Type; // Pre-defined in typedefs.h, ignore warning

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

    Type* parent;
};

void initTypesMap(TypesMap* map);

Type* typeClone(Type* orig);

Type* newType(const char* name);

Type* newTypeWithParent(const char* name, Type* parentType);

Type* newTypeWithArgs(const char* name, int numTypeArgs, Type** typeArgs, const char** typeArgNames);

Type* newTypeWithParentAndArgs(const char* name, Type* parentType, int numTypeArgs, Type** typeArgs,
                               const char** typeArgNames);

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

bool typeEq(Type* targetType, Type* currentType);

typedef struct TypeExpr TypeExpr; // Generates warning in ast.h for redefinition

Type* resolveType(TypeExpr* typeExpr, TypesMap* types);

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
