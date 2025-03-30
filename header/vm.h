#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#define STACK_MAX (256)
typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;
typedef struct {
  Chunk *chunk;//Chunk is a list of instruction,
	       //So Vm is series of list of instruction.
  uint8_t *ip; //ip stands for instruction pointer.So it is instruction pointer for each chunks.
  Value stack[STACK_MAX];
  Value *stackTop; // A pointer is kept instead of index,because it is faster to
                   // dereference.
} VM;
void initVM(void);
void freeVM(void);
void push(Value value);
Value pop(void);
InterpretResult interpret(const char* source);
#endif
