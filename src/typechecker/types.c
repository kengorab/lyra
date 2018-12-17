#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser/ast.h"
#include "types.h"

const char* primitiveTypes[] = {PRIMITIVE_TYPE_TYPES};

static Type _typeString = {
    .type = PRIMITIVE_TYPE_STRING, .name = "String", .numTypeArgs = 0, .typeArgs = NULL, .typeArgNames = NULL
};
static Type _typeInt = {
    .type = PRIMITIVE_TYPE_INT, .name = "Int", .numTypeArgs = 0, .typeArgs = NULL, .typeArgNames = NULL
};
static Type _typeDouble = {
    .type = PRIMITIVE_TYPE_DOUBLE, .name = "Double", .numTypeArgs = 0, .typeArgs = NULL, .typeArgNames = NULL
};
static Type _typeBool = {
    .type = PRIMITIVE_TYPE_BOOL, .name = "Bool", .numTypeArgs = 0, .typeArgs = NULL, .typeArgNames = NULL
};
static Type _typeNil = {
    .type = PRIMITIVE_TYPE_NIL, .name = "Nil", .numTypeArgs = 0, .typeArgs = NULL, .typeArgNames = NULL
};
static Type _typeAny = {
    .type = PRIMITIVE_TYPE_ANY, .name = "Any", .numTypeArgs = 0, .typeArgs = NULL, .typeArgNames = NULL
};
static Type _typeUnit = {
    .type = PRIMITIVE_TYPE_UNIT, .name = "Unit", .numTypeArgs = 0, .typeArgs = NULL, .typeArgNames = NULL
};

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

    listType->typeArgs = calloc(1, sizeof(Type*));
    listType->typeArgs[0] = typeArg;

    listType->typeArgNames = calloc(1, sizeof(char*));
    listType->typeArgNames[0] = "T";
    return listType;
}

Type* typeObj(const char* name, int numFields, Type** fieldTypes, const char** fieldNames) {
    Type* funcType = malloc(sizeof(Type));
    funcType->type = PRIMITIVE_TYPE_NONPRIMITIVE;
    funcType->name = name == NULL ? "Obj" : name;
    funcType->numTypeArgs = numFields;

    funcType->typeArgs = calloc((size_t) numFields, sizeof(Type*));
    memcpy(funcType->typeArgs, fieldTypes, numFields * sizeof(Type*));

    funcType->typeArgNames = calloc((size_t) numFields, sizeof(char*));
    memcpy(funcType->typeArgNames, fieldNames, numFields * sizeof(char*));
    return funcType;
}

Type* typeFunction(Type* returnType, int numArgs, Type** argTypes, const char** paramNames) {
    Type* funcType = malloc(sizeof(Type));
    funcType->type = PRIMITIVE_TYPE_NONPRIMITIVE;
    funcType->name = "Function";
    funcType->numTypeArgs = numArgs + 1;

    funcType->typeArgs = calloc((size_t) numArgs + 1, sizeof(Type*));
    funcType->typeArgs[0] = returnType;
    memcpy(funcType->typeArgs + 1, argTypes, numArgs * sizeof(Type*));

    funcType->typeArgNames = calloc((size_t) numArgs + 1, sizeof(char*));
    funcType->typeArgNames[0] = "_ret";
    memcpy(funcType->typeArgNames + 1, paramNames, numArgs * sizeof(char*));
    return funcType;
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
