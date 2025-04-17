#include "memory.h"
#include "common.h"
#include "value.h"
#include "vm.h"
#include <object.h>
#include <stdlib.h>

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

static void freeObject(Obj *object) {
  switch (object->type) {

  case OBJ_STRING: {
    ObjString *string = (ObjString *)object;
    FREE_ARRAY(char, string->chars, string->length + 1);
    FREE(ObjString, object);
    break;
  }
  }
}

void *reallocate(void *pointer, size_t oldsize,
                 size_t newsize) { // It basically says not to new library
                                   // function openly,You don't know when you
                                   // need to edit it.
                                   //
  (void)oldsize; // I don't know what the oldSize is used for,But it is in the
                 // book.
  if (newsize == 0) {
    free(pointer);
    return NULL;
  }
  void *result = realloc(pointer, newsize);
  if (result == NULL) {
    exit(1);
  }
  return result;
}

extern VM vm;
void freeObjects(void) {
  Obj *object = vm.objects;
  while (object != NULL) {
    Obj *next = object->next;
    freeObject(object);
    object = next;
  }
}
