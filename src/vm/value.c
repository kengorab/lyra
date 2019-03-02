#include <stdlib.h>
#include <stdio.h>

#include "value.h"

void printValue(Value value) {
    printf("%g", value);
}

void initValueArray(ValueArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void writeValueArray(ValueArray* array, Value value) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = oldCapacity < 8 ? 8 : (oldCapacity * 2);
        array->values = realloc(array->values, (size_t) array->capacity * sizeof(Value));
    }

    array->values[array->count++] = value;
}

void freeValueArray(ValueArray* array) {
    free(array->values);
    initValueArray(array);
}
