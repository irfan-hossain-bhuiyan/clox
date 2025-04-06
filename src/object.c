#include "object.h"
#include "memory.h"
#include "value.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ALLOCATE_OBJ(type, objectType)                                         \
  (type *)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(size_t size, ObjType type) {
  Obj *object = (Obj *)reallocate(NULL, 0, size);
  object->type = type;
  return object;
}

static ObjString *allocateString(char *chars, uintptr_t length) {
  ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  string->length = length;
  string->chars = chars;

  return string;
}
// takeString takes the pointer and set
// it as string,So it works as move
ObjString *takeString(char *chars, uintptr_t length) {
  return allocateString(chars, length);
}
// copyString takes the pointer,and copy it.
// So it doesn't move
ObjString *copyString(const char *chars, uintptr_t length) {
  char *heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  return allocateString(heapChars, length);
}
void objectToString(char *output,const Value value) {
  switch (OBJ_TYPE(value)) {

  case OBJ_STRING:
    sprintf(output, "%s", AS_CSTRING(value));
    break;
  }
}

void printObject(Value value) {
  switch (OBJ_TYPE(value)) {

  case OBJ_STRING:
    printf("%s", AS_CSTRING(value));
    break;
  }
}
