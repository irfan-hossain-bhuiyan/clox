#include "value.h"
#include "memory.h"
#include <object.h>
#include <stdbool.h>
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

void printValue(Value value) {
  switch (value.type) {
  case VAL_BOOL:
    printf(AS_BOOL(value) ? "true" : "false");
    break;
  case VAL_NIL:
    printf("nill");
    break;
  case VAL_NUMBER:
    printf("%g", AS_NUMBER(value));
    break;
  case VAL_OBJ:
    printObject(value);
    break;
  }
}
bool valuesEqual(Value valueA, Value valueB) {
  if (valueA.type != valueB.type) {
    return false;
  }
  switch (valueA.type) {
  case VAL_BOOL:
    return AS_BOOL(valueA) == AS_BOOL(valueB);
  case VAL_NIL:
    return true;
  case VAL_NUMBER:
    return AS_NUMBER(valueA) == AS_NUMBER(valueB);
  case VAL_OBJ: {
    ObjString *aString = AS_STRING(valueA);
    ObjString *bString = AS_STRING(valueB);
    return aString->length == bString->length &&
           memcmp(aString->chars, bString->chars, aString->length) == 0;
  }
  }
}
