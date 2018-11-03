#include <stdlib.h>
#include <string.h>

#include "types.h"

const char* primitiveTypes[] = {PRIMITIVE_TYPE_TYPES};

Type typeString = {.type = PRIMITIVE_TYPE_STRING, .name = "String"};

Type typeInt = {.type = PRIMITIVE_TYPE_INT, .name = "Int"};

Type typeDouble = {.type = PRIMITIVE_TYPE_DOUBLE, .name = "Double"};

Type typeBool = {.type = PRIMITIVE_TYPE_BOOL, .name = "Bool"};

Type typeNil = {.type = PRIMITIVE_TYPE_NIL, .name = "Nil"};

inline bool typeEq(Type t1, Type t2) {
    return strcmp(t1.name, t2.name) == 0;
}
