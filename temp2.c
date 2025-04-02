#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"
#include <assert.h>
#include <compiler.h>
#include <logging.h>
#include <scanner.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
bool RUNNING_WITH_CTEST;
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

// A simple function to test; alternatively, include your header file if this
// function is declared there.
const char *EXAMPLE_CODE = "var a=10;\n"
                           "var b=15;\n";
typedef struct {
  TokenType type;
  int line;
} DToken;
const DToken EXAMPLE_TOKEN[] = {
    {TOKEN_VAR, 1},    {TOKEN_IDENTIFIER, 1}, {TOKEN_EQUAL, 1},
    {TOKEN_NUMBER, 1}, {TOKEN_SEMICOLON, 1},

    {TOKEN_VAR, 2},    {TOKEN_IDENTIFIER, 2}, {TOKEN_EQUAL, 2},
    {TOKEN_NUMBER, 2}, {TOKEN_SEMICOLON, 2},  {TOKEN_EOF, 3},
};

void tokenAssert(void) {
  initScanner(EXAMPLE_CODE);
  int i = 0;
  while (true) {
    Token generatedToken = scanToken();
    DToken expectedToken = EXAMPLE_TOKEN[i];
    fprintf(stdout, "  Expected: (line: %d, type: %s)\n", expectedToken.line,
            tokenTypeToString(expectedToken.type));
    fprintf(stdout, "  Generated: (line: %d, type: %s)\n", generatedToken.line,
            tokenTypeToString(generatedToken.type));
    if (RUNNING_WITH_CTEST) {
      if (generatedToken.line != expectedToken.line ||
          generatedToken.type != expectedToken.type) {
        fprintf(stderr, "Error at token index %d:\n", i);
        exit(EXIT_FAILURE); // Causes the test to fail in CTest
      }
    }
    if (generatedToken.type == TOKEN_EOF) {
      break;
    }
    i++;
  }
}

void expressionAssert(const char *expression, const OpCode *expressionOpcodes,
                      const Value *expressionConstants) {
  Chunk chunk;
  initChunk(&chunk);
  compile(expression, &chunk);
  for (int i = 0; i < chunk.count; i++) {
    OpCode generatedInstruction = chunk.code[i];
    OpCode expectedInstruction = expressionOpcodes[i];
    fprintf(stdout, " Generated: opcode:%s \n",
            opcodeToString(generatedInstruction));
    fprintf(stdout, " Expected:  opcode:%s \n",
            opcodeToString(expectedInstruction));

    if (RUNNING_WITH_CTEST) {
      if (chunk.code[i] != expressionOpcodes[i]) {
        fprintf(stderr, "Error at opcode index %d\n", i);
        exit(EXIT_FAILURE);
      }
    }
  }
  for (int i = 0; i < chunk.constants.count; i++) {
    Value generatedConstant = chunk.constants.values[i];
    Value expectedConstant = expressionConstants[i];
    if (RUNNING_WITH_CTEST) {
      if (valuesEqual(generatedConstant, expectedConstant)) {
        fprintf(stderr, "Error at opcode index %d\n", i);
        exit(EXIT_FAILURE);
      }
    }
  }
  freeChunk(&chunk);
}
void exprEvalAssert(const char *expr, Value expectedOutput) {
  printf("\n\nThe expression: \"%s\"\n", expr);
  InterpretResult result = interpret(expr);
  switch (result) {

  case INTERPRET_OK:
    break;
  case INTERPRET_COMPILE_ERROR:
    fprintf(stderr, "tests outputs compile error.");
    exit(EXIT_FAILURE);
  case INTERPRET_RUNTIME_ERROR:
    fprintf(stderr, "tests outputs have runtime error.");
    exit(EXIT_FAILURE);
    break;
  }
  Value returnedOutput = getLastReturn();
  assert(valuesEqual(returnedOutput, expectedOutput));
}
void exprErrorAssert(const char *expr, InterpretResult error) {
  InterpretResult result = interpret(expr);
  assert(error == result);
}

void vmAssert(void) {
  initVM();
  exprEvalAssert("2+3*(3*3)", NUMBER_VAL(29));
  exprEvalAssert("2<3.1", BOOL_VAL(true));
  exprEvalAssert("2>3.1", BOOL_VAL(false));
  exprEvalAssert("2>=3.1", BOOL_VAL(false));
  exprEvalAssert("2<=3.1", BOOL_VAL(true));
  exprEvalAssert("2*3<=3.1415*2", BOOL_VAL(true));
  exprEvalAssert("false!=false", BOOL_VAL(false));
  exprEvalAssert("false==false", BOOL_VAL(true));
  exprEvalAssert("nil==nil", BOOL_VAL(true));
  exprEvalAssert("nil<=nil", BOOL_VAL(false));
  exprEvalAssert("3.14159", NUMBER_VAL(3.14159));
  exprEvalAssert("true", BOOL_VAL(true));
  exprErrorAssert("1++2--3", INTERPRET_COMPILE_ERROR);
  exprErrorAssert("1+--3", INTERPRET_COMPILE_ERROR);
  exprErrorAssert("true*3+2", INTERPRET_RUNTIME_ERROR);
  exprErrorAssert("true+true", INTERPRET_RUNTIME_ERROR);
}

void tokenTest(void) {
  tokenAssert();
  greenPrint("\nTokens is working.\n");
}
int main(int argc, char *argV[]) {
  // Setting env variable
  // Basic assertions for testing
  tokenTest();

  const char *EXPRESSION0 = "12+15*3--12";
  const OpCode EXPRESSION_OPCODE0[] = {
      OP_CONSTANT, 0,      OP_CONSTANT, 1, OP_CONSTANT, 2,
      OP_MULTIPLY, OP_ADD, OP_CONSTANT, 3, OP_NEGATE,   OP_SUBSTRACT,
  };
  const Value EXPRESSION_CONSTANT0[] = {NUMBER_VAL(12), NUMBER_VAL(15),
                                        NUMBER_VAL(3), NUMBER_VAL(12)};

  expressionAssert(EXPRESSION0, EXPRESSION_OPCODE0, EXPRESSION_CONSTANT0);

  bluePrint("\nExperssion 0 passed\n");
  const char *EXPRESSION1 = "2+3*(3*3)";
  const OpCode EXPRESSION_OPCODE1[] = {OP_CONSTANT, 0,           OP_CONSTANT, 1,
                                       OP_CONSTANT, 2,           OP_CONSTANT, 3,
                                       OP_MULTIPLY, OP_MULTIPLY, OP_ADD};
  const Value EXPRESSION_CONSTANT1[] = {NUMBER_VAL(2), NUMBER_VAL(3),
                                        NUMBER_VAL(3), NUMBER_VAL(3)};

  expressionAssert(EXPRESSION1, EXPRESSION_OPCODE1, EXPRESSION_CONSTANT1);
  bluePrint("\nExperssion 1 passed\n");

  const char *EXPRESSION2 = "2<3";
  const OpCode EXPRESSION_OPCODE2[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1, OP_LESS,
  };
  const Value EXPRESSION_CONSTANT2[] = {
      NUMBER_VAL(2),
      NUMBER_VAL(3),
  };
  expressionAssert(EXPRESSION2, EXPRESSION_OPCODE2, EXPRESSION_CONSTANT2);

  const char *EXPRESSION3 = "3<=3";
  const OpCode EXPRESSION_OPCODE3[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1, OP_GREATER, OP_NOT,
  };
  const Value EXPRESSION_CONSTANT3[] = {
      NUMBER_VAL(3),
      NUMBER_VAL(3),
  };
  expressionAssert(EXPRESSION3, EXPRESSION_OPCODE3, EXPRESSION_CONSTANT3);

  greenPrint("\nExpression parser is working\n");

  vmAssert();
  greenPrint("\n VM is working \n");
  // Evaluating VM,If it works
  return 0;
}
