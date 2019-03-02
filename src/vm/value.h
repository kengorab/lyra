#ifndef CLYRA_VALUE_H
#define CLYRA_VALUE_H

#include "common.h"

typedef double Value;

void printValue(Value value);

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* array);

void writeValueArray(ValueArray* array, Value value);

void freeValueArray(ValueArray* array);

#endif //CLYRA_VALUE_H
