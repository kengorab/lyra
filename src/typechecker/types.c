#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser/ast.h"
#include "types.h"

const char* primitiveTypes[] = {PRIMITIVE_TYPE_TYPES};

static Type _typeString = {.type = PRIMITIVE_TYPE_STRING, .name = "String", .numTypeArgs = 0, .typeArgs = NULL};
static Type _typeInt = {.type = PRIMITIVE_TYPE_INT, .name = "Int", .numTypeArgs = 0, .typeArgs = NULL};
static Type _typeDouble = {.type = PRIMITIVE_TYPE_DOUBLE, .name = "Double", .numTypeArgs = 0, .typeArgs = NULL};
static Type _typeBool = {.type = PRIMITIVE_TYPE_BOOL, .name = "Bool", .numTypeArgs = 0, .typeArgs = NULL};
static Type _typeNil = {.type = PRIMITIVE_TYPE_NIL, .name = "Nil", .numTypeArgs = 0, .typeArgs = NULL};
static Type _typeAny = {.type = PRIMITIVE_TYPE_ANY, .name = "Any", .numTypeArgs = 0, .typeArgs = NULL};
static Type _typeUnit = {.type = PRIMITIVE_TYPE_UNIT, .name = "Unit", .numTypeArgs = 0, .typeArgs = NULL};

Type* typeString() { return &_typeString; }

Type* typeInt() { return &_typeInt; }

Type* typeDouble() { return &_typeDouble; }

Type* typeBool() { return &_typeBool; }

Type* typeNil() { return &_typeNil; }

Type* typeAny() { return &_typeAny; }

Type* typeUnit() { return &_typeUnit; }

Type* typeList(Type* typeArg) {
    Type* listType = malloc(sizeof(Type));
    listType->type = PRIMITIVE_TYPE_NONPRIMITIVE;
    listType->name = "List";
    listType->numTypeArgs = 1;
    listType->typeArgs = malloc(sizeof(Type*) * 1);
    listType->typeArgs[0] = typeArg;
    return listType;
}

bool typeEq(Type* t1, Type* t2) {
    return t1 == t2;
}

Type* resolveType(TypeExpr* typeExpr) {
    switch (typeExpr->type) {
        case TYPE_EXPR_BASIC_TYPE: {
            const char* name = typeExpr->as.basicType.name->name;
            if (strcmp("String", name) == 0) {
                return typeString();
            } else if (strcmp("Int", name) == 0) {
                return typeInt();
            } else if (strcmp("Double", name) == 0) {
                return typeDouble();
            } else if (strcmp("Bool", name) == 0) {
                return typeBool();
            } else if (strcmp("Unit", name) == 0) {
                return typeUnit();
            } else {
                Type* type = malloc(sizeof(Type*));
                type->type = PRIMITIVE_TYPE_NONPRIMITIVE;
                type->name = name;
                return type;
            }
        }
        default: {
            printf("Unknown type: %s; exiting\n", tokenGetValue(typeExpr->token));
            exit(1);
        }
    }
}
