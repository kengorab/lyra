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

void initTypesMap(TypesMap* map) {
    add_TypesMap(map, "Int", &_typeInt);
    add_TypesMap(map, "String", &_typeString);
    add_TypesMap(map, "Double", &_typeDouble);
    add_TypesMap(map, "Bool", &_typeBool);
    add_TypesMap(map, "Any", &_typeAny);
    add_TypesMap(map, "Unit", &_typeUnit);

    add_TypesMap(map, "List", typeList(&_typeAny));
}

Type* newTypeWithParent(const char* name, Type* parentType) {
    Type* type = malloc(sizeof(Type));

    type->type = PRIMITIVE_TYPE_NONPRIMITIVE;
    type->name = name;
    type->numTypeArgs = 0;
    type->typeArgs = NULL;
    type->typeArgNames = NULL;
    type->parent = parentType;

    return type;
}

Type* newType(const char* name) {
    return newTypeWithParent(name, NULL);
}

Type* newTypeWithParentAndArgs(const char* name, Type* parentType, int numTypeArgs, Type** typeArgs,
                               const char** typeArgNames) {
    Type* t = newType(name);
    t->numTypeArgs = numTypeArgs;
    t->typeArgs = typeArgs;
    t->typeArgNames = typeArgNames;
    t->parent = parentType;
    return t;
}

Type* newTypeWithArgs(const char* name, int numTypeArgs, Type** typeArgs, const char** typeArgNames) {
    return newTypeWithParentAndArgs(name, NULL, numTypeArgs, typeArgs, typeArgNames);
}

bool typeEq(Type* targetType, Type* currentType) {
    if (targetType->type == PRIMITIVE_TYPE_NONPRIMITIVE && currentType->type == PRIMITIVE_TYPE_NONPRIMITIVE) {
        if (strcmp(targetType->name, currentType->name) != 0) {
            return false;
        }
        if (targetType->numTypeArgs != currentType->numTypeArgs) {
            return false;
        }
        for (int i = 0; i < targetType->numTypeArgs; ++i) {
            if (!typeEq(targetType->typeArgs[i], currentType->typeArgs[i])) {
                return false;
            }
        }
        return true;
    }

    if (targetType->type != PRIMITIVE_TYPE_NONPRIMITIVE && currentType->type != PRIMITIVE_TYPE_NONPRIMITIVE) {
        return targetType == currentType;
    }

    // TODO: do this test in the both-non-primitive branch too!

    Type* parent = currentType->parent;
    while (parent != NULL) {
        if (typeEq(targetType, parent)) {
            return true;
        }
        parent = parent->parent;
    }
    return false;
}

Type* resolveType(TypeExpr* typeExpr, TypesMap* typesMap) {
    switch (typeExpr->type) {
        case TYPE_EXPR_BASIC_TYPE: {
            const char* name = typeExpr->as.basicType.name->name;

            Type* type;
            if (get_TypesMap(typesMap, name, &type) != MAP_OK) {
                return NULL; // Unknown type, throw error up at call site
            }

            for (int i = 0; i < typeExpr->numArgs; ++i) {
                Type* arg = resolveType(typeExpr->typeArgs[i], typesMap);
                if (arg == NULL) return NULL;

                type->typeArgs[i] = arg;
            }

            return type;

//            if (strcmp("String", name) == 0) {
//                return typeString();
//            } else if (strcmp("Int", name) == 0) {
//                return typeInt();
//            } else if (strcmp("Double", name) == 0) {
//                return typeDouble();
//            } else if (strcmp("Bool", name) == 0) {
//                return typeBool();
//            } else if (strcmp("Unit", name) == 0) {
//                return typeUnit();
//            }
//
//            Type* type = malloc(sizeof(Type*));
//            type->type = PRIMITIVE_TYPE_NONPRIMITIVE;
//            type->name = name;
//            return type;
        }
        default: {
            printf("Unknown type: %s; exiting\n", tokenGetValue(typeExpr->token));
            exit(1);
        }
    }
}
