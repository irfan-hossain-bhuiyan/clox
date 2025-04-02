#include "vm.h"
#include "chunk.h"
#include "compiler.h"
#include "debug.h"
#include "value.h"
#include <__stdarg_va_list.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
VM vm;

// Clear the entire stack
static void resetStack(void) {
  vm.stackTop = vm.stack; // This means the stack is empty.
}
void initVM(void) { resetStack(); }
void freeVM(void) {}
static Value peek(int distance) { return vm.stackTop[-1 - distance]; }
static void runtimeError(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);
  size_t instructioNumber =
      vm.ip - vm.chunk->code - 1; // The instruction always points to the next
                                  // element that's goi ng to be executed.
  int lines = vm.chunk->lines[instructioNumber];
  fprintf(stderr, "[line %d] in script \n", lines);
  resetStack();
}

static bool isFalsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}
static Value lastReturned = NIL_VAL;
// Save the last return output of the code
static void saveReturn(void) { lastReturned = peek(0); }
Value getLastReturn(void) { return lastReturned; }
static bool valuesEqual(Value valueA, Value valueB) {
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
  }
}
static InterpretResult run(void) {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op)                                               \
  do {                                                                         \
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {                          \
      runtimeError("Operands must be numbers");                                \
      return INTERPRET_RUNTIME_ERROR;                                          \
    }                                                                          \
    double b = AS_NUMBER(pop());                                               \
    double a = AS_NUMBER(pop());                                               \
    push(valueType(a op b));                                                   \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    // Dispalying the stack in debig mode.
    printf("Stack:         ");
    printf("[");
    for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
      printValue(*slot);
      printf(",");
    }
    printf("]\n");
    // End of display
    printf("Instruction:        ");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_NEGATE: {
      if (!IS_NUMBER(peek(0))) {
        runtimeError("Operand must be a number.");
        return INTERPRET_RUNTIME_ERROR;
      }
      push(NUMBER_VAL(-AS_NUMBER(pop())));
      break;
    }
    case OP_ADD: {
      BINARY_OP(NUMBER_VAL, +);
      break;
    }
    case OP_SUBSTRACT: {
      BINARY_OP(NUMBER_VAL, -);
      break;
    }
    case OP_DIVIDE: {
      BINARY_OP(NUMBER_VAL, /);
      break;
    }
    case OP_MULTIPLY: {
      BINARY_OP(NUMBER_VAL, *);
      break;
    }
    case OP_GREATER: {
      BINARY_OP(BOOL_VAL, >);
      break;
    }
    case OP_LESS: {
      BINARY_OP(BOOL_VAL, <);
      break;
    }

    case OP_NIL:
      push(NIL_VAL);
      break;
    case OP_TRUE:
      push(BOOL_VAL(true));
      break;
    case OP_FALSE:
      push(BOOL_VAL(false));
      break;
    case OP_NOT:
      push(BOOL_VAL(isFalsey(pop())));
      break;
    case OP_EQUAL: {
      Value b = pop();
      Value a = pop();
      push(BOOL_VAL(valuesEqual(a, b)));
      break;
    }
    case OP_RETURN: {
      saveReturn();
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
    }
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
}
InterpretResult interpret(const char *source) {
  Chunk chunk;
  initChunk(&chunk);
  if (!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }
  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;
  InterpretResult result = run();
  freeChunk(&chunk);
  return result;
}
void push(Value value) { *(vm.stackTop++) = value; }
Value pop(void) { return *(--vm.stackTop); }
