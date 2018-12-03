#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser/ast.h"
#include "types.h"

const char* primitiveTypes[] = {PRIMITIVE_TYPE_TYPES};

static Type _typeString = {.type = PRIMITIVE_TYPE_STRING, .name = "String"};
static Type _typeInt = {.type = PRIMITIVE_TYPE_INT, .name = "Int"};
static Type _typeDouble = {.type = PRIMITIVE_TYPE_DOUBLE, .name = "Double"};
static Type _typeBool = {.type = PRIMITIVE_TYPE_BOOL, .name = "Bool"};
static Type _typeNil = {.type = PRIMITIVE_TYPE_NIL, .name = "Nil"};
static Type _typeUnit = {.type = PRIMITIVE_TYPE_UNIT, .name = "Unit"};

Type* typeString() { return &_typeString; }

Type* typeInt() { return &_typeInt; }

Type* typeDouble() { return &_typeDouble; }

Type* typeBool() { return &_typeBool; }

Type* typeNil() { return &_typeNil; }

Type* typeUnit() { return &_typeUnit; }

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
