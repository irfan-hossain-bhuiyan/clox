#include "object.h"
#include "memory.h"
#include "value.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ALLOCATE_OBJ(type, objectType)                                         \
  (type *)allocateObject(sizeof(type), objectType)

extern VM vm;
// This is the only function to create object.
static Obj *allocateObject(size_t size, ObjType type) {
  Obj *object = (Obj *)reallocate(NULL, 0, size);
  object->type = type;
  object->next = vm.objects;
  vm.objects = object;
  return object;
}

static ObjString *allocateString(char *chars, uintptr_t length, uint32_t hash) {
  ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  string->length = length;
  string->chars = chars;
  string->hash = hash;

  return string;
}

static uint32_t hashString(const char *key, uintptr_t length) {
  uint32_t hash = 2166136261u;
  for (uintptr_t i = 0; i < length; i++) {
    hash ^= key[i];
    hash *= 16777619;
  }
  return hash;
}
// takeString takes the pointer and set
// it as string,So it works as move
ObjString *takeString(char *chars, uintptr_t length) {
  uint32_t hash = hashString(chars, length);
  return allocateString(chars, length, hash);
}
// copyString takes the pointer,and copy it.
// So it doesn't move
ObjString *copyString(const char *chars, uintptr_t length) {
  uint32_t hash = hashString(chars, length);
  char *heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  return allocateString(heapChars, length, hash);
}
void objectToString(char *output, const Value value) {
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
