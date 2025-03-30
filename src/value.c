#include "value.h"
#include "memory.h"
#include <stdio.h>
void writeValueArray(ValueArray *array, Value value) {
  if (array->capacity < array->count + 1) {
    int oldcapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldcapacity);
    array->values =
        GROW_ARRAY(Value, array->values, oldcapacity, array->capacity);
  }
  array->values[array->count] = value;
  array->count++;
}

void initValueArray(ValueArray *array) {
  array->values = NULL;
  array->count = 0;
  array->capacity = 0;
}

void freeValueArray(ValueArray *array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  initValueArray(array);
}

void printValue(Value value) { printf("%g", value); }
