
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"
#include <__stdarg_va_list.h>
#include <assert.h>
#include <compiler.h>
#include <logging.h>
#include <scanner.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static bool haltOnFailure = true;
static void failedExit(const char *errorReport, ...) {
  va_list args;
  va_start(args, errorReport);
  fprintf(stderr, errorReport, args);
  va_end(args);
  if (haltOnFailure) {
    exit(EXIT_FAILURE);
  }
}
#define ASSERT(x)                                                              \
  do {                                                                         \
    if (!(x)) {                                                                \
      failedExit("ASSERT(" #x ")");                                            \
    }                                                                          \
  } while (false);

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
typedef struct {
  TokenType type;
  int line;
} DToken;

void tokenAssert(const char *str, const DToken *tokens) {
  bluePrint("Code is :%s", str);
  initScanner(str);
  int i = 0;
  while (true) {
    Token generatedToken = scanToken();
    DToken expectedToken = tokens[i];
    fprintf(stdout, "  Expected: (line: %d, type: %s)\n", expectedToken.line,
            tokenTypeToString(expectedToken.type));
    fprintf(stdout, "  Generated: (line: %d, type: %s , text: %.*s)\n",
            generatedToken.line, tokenTypeToString(generatedToken.type),
            generatedToken.length, generatedToken.start);

    if (generatedToken.line != expectedToken.line ||
        generatedToken.type != expectedToken.type) {
      failedExit("Error at token index %d:\n",
                 i); // Causes the test to fail in CTest
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
  bool allOk = compile(expression, &chunk);
  bluePrint("\nCode: %s\n", expression);
  if (!allOk) {
    failedExit("Trying to compile expression that are not valid.");
  }
  for (int i = 0; i < chunk.count; i++) {
    OpCode generatedInstruction = chunk.code[i];
    OpCode expectedInstruction = expressionOpcodes[i];
    fprintf(stdout, " Generated: opcode:%s \n",
            opcodeToString(generatedInstruction));
    fprintf(stdout, " Expected:  opcode:%s \n",
            opcodeToString(expectedInstruction));
    if (chunk.code[i] != expressionOpcodes[i]) {
      failedExit("Error at opcode index %d\n", i);
    }

    // Different printing if the next code is constant pointer
    if (generatedInstruction == OP_CONSTANT) {
      i++;
      generatedInstruction = chunk.code[i];
      expectedInstruction = expressionOpcodes[i];
      Value generatedConstant = chunk.constants.values[generatedInstruction];
      Value expectedConstant = expressionConstants[expectedInstruction];
      fprintf(stdout, " Generated: ConstantPointer:%d --> %s\n",
              generatedInstruction, valueToString(generatedConstant).core);
      fprintf(stdout, " Expected:  ConstantPointer:%d --> %s\n",
              expectedInstruction, valueToString(expectedConstant).core);
      if (chunk.code[i] != expressionOpcodes[i]) {
        failedExit("Constant pointer mismatch %d\n", i);
      }
      if (!valuesEqual(generatedConstant, expectedConstant)) {
        failedExit("Constant mismatched: %d\n", i);
      }
    }
  }

  // Checking for constant
  freeChunk(&chunk);
}
void exprEvalAssert(const char *expr, Value expectedOutput) {

  initVM();
  printf("\n\nThe expression: \"%s\"\n", expr);
  InterpretResult result = interpret(expr);
  switch (result) {

  case INTERPRET_OK:
    break;
  case INTERPRET_COMPILE_ERROR:
    failedExit("tests outputs compile error.");
  case INTERPRET_RUNTIME_ERROR:
    failedExit("tests outputs have runtime error.");
    break;
  }
  Value returnedOutput = getLastReturn();
  ASSERT(valuesEqual(returnedOutput, expectedOutput));
  freeVM();
}
void exprErrorAssert(const char *expr, InterpretResult error) {
  initVM();
  printf(TO_BLUE("Code is->\"%s\"\n"), expr);
  InterpretResult result = interpret(expr);
  ASSERT(error == result);
  printf("All ok\n");
  freeVM();
}

void vmTest(void) {
  exprEvalAssert("2+3*(3*3)", NUMBER_VAL(29));
  exprEvalAssert("2<3.1", BOOL_VAL(true));
  exprEvalAssert("2>3.1", BOOL_VAL(false));
  exprEvalAssert("2>=3.1", BOOL_VAL(false));
  exprEvalAssert("2<=3.1", BOOL_VAL(true));
  exprEvalAssert("2*3<=3.1415*2", BOOL_VAL(true));
  exprEvalAssert("false!=false", BOOL_VAL(false));
  exprEvalAssert("false==false", BOOL_VAL(true));
  exprEvalAssert("nil==nil", BOOL_VAL(true));
  exprEvalAssert("3.14159", NUMBER_VAL(3.14159));
  exprEvalAssert("true", BOOL_VAL(true));
  exprEvalAssert("1+--3", NUMBER_VAL(4));
  exprErrorAssert("1++2--3", INTERPRET_COMPILE_ERROR);
  exprErrorAssert("true*3+2", INTERPRET_RUNTIME_ERROR);
  exprErrorAssert("true+true", INTERPRET_RUNTIME_ERROR);
  exprErrorAssert("nil<=nil", INTERPRET_RUNTIME_ERROR);

  greenPrint("\n VM is working \n");
}

static void tokenTest(void) {
  const char *CODE = "var a=10;\n"
                     "var b=15;\n";
  const DToken TOKENS[] = {
      {TOKEN_VAR, 1},    {TOKEN_IDENTIFIER, 1}, {TOKEN_EQUAL, 1},
      {TOKEN_NUMBER, 1}, {TOKEN_SEMICOLON, 1},

      {TOKEN_VAR, 2},    {TOKEN_IDENTIFIER, 2}, {TOKEN_EQUAL, 2},
      {TOKEN_NUMBER, 2}, {TOKEN_SEMICOLON, 2},  {TOKEN_EOF, 3},
  };

  tokenAssert(CODE, TOKENS);

  const char *CODE1 = "a>b";
  const DToken TOKENS1[] = {{TOKEN_IDENTIFIER, 1},
                            {TOKEN_GREATER, 1},
                            {TOKEN_IDENTIFIER, 1},
                            {TOKEN_EOF, 1}};

  tokenAssert(CODE1, TOKENS1);
  const char *CODE2 = "a>=b";
  const DToken TOKENS2[] = {{TOKEN_IDENTIFIER, 1},
                            {TOKEN_GREATER_EQUAL, 1},
                            {TOKEN_IDENTIFIER, 1},
                            {TOKEN_EOF, 1}};
  tokenAssert(CODE2, TOKENS2);

  const char *CODE3 = "a>b==\"true\"\n\n;";
  const DToken TOKENS3[] = {
      {TOKEN_IDENTIFIER, 1},  {TOKEN_GREATER, 1}, {TOKEN_IDENTIFIER, 1},
      {TOKEN_EQUAL_EQUAL, 1}, {TOKEN_STRING, 1},  {TOKEN_SEMICOLON, 3},
      {TOKEN_EOF, 3},
  };
  tokenAssert(CODE3, TOKENS3);

  const char *CODE4 = "false!=false";
  const DToken TOKENS4[] = {{TOKEN_FALSE, 1},
                            {TOKEN_BANG_EQUAL, 1},
                            {TOKEN_FALSE, 1},
                            {TOKEN_EOF, 1}};

  tokenAssert(CODE4, TOKENS4);

  const char *CODE5 = "2<3";
  const DToken TOKENS5[] = {
      {TOKEN_NUMBER, 1}, {TOKEN_LESS, 1}, {TOKEN_NUMBER, 1}, {TOKEN_EOF, 1}};
  tokenAssert(CODE5, TOKENS5);
  greenPrint("\nTokens is working.\n");
}
static void expressionCompilerTest(void) {
  const char *EXPRESSION0 = "12+15*3--12";
  const OpCode EXPRESSION_OPCODE0[] = {
      OP_CONSTANT, 0,      OP_CONSTANT, 1, OP_CONSTANT, 2,
      OP_MULTIPLY, OP_ADD, OP_CONSTANT, 3, OP_NEGATE,   OP_SUBSTRACT,
      OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT0[] = {NUMBER_VAL(12), NUMBER_VAL(15),
                                        NUMBER_VAL(3), NUMBER_VAL(12)};

  expressionAssert(EXPRESSION0, EXPRESSION_OPCODE0, EXPRESSION_CONSTANT0);

  bluePrint("\nExperssion 0 passed\n");
  const char *EXPRESSION1 = "2+3*(3*3)";
  const OpCode EXPRESSION_OPCODE1[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1,           OP_CONSTANT, 2,
      OP_CONSTANT, 3, OP_MULTIPLY, OP_MULTIPLY, OP_ADD,      OP_RETURN};
  const Value EXPRESSION_CONSTANT1[] = {NUMBER_VAL(2), NUMBER_VAL(3),
                                        NUMBER_VAL(3), NUMBER_VAL(3)};

  expressionAssert(EXPRESSION1, EXPRESSION_OPCODE1, EXPRESSION_CONSTANT1);
  bluePrint("\nExperssion 1 passed\n");

  const char *EXPRESSION2 = "2<3";
  const OpCode EXPRESSION_OPCODE2[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1, OP_LESS, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT2[] = {
      NUMBER_VAL(2),
      NUMBER_VAL(3),
  };
  expressionAssert(EXPRESSION2, EXPRESSION_OPCODE2, EXPRESSION_CONSTANT2);

  const char *EXPRESSION3 = "3<=3";
  const OpCode EXPRESSION_OPCODE3[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1, OP_GREATER, OP_NOT, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT3[] = {
      NUMBER_VAL(3),
      NUMBER_VAL(3),
  };
  expressionAssert(EXPRESSION3, EXPRESSION_OPCODE3, EXPRESSION_CONSTANT3);

  const char *EXPRESSION4 = "false!=false";
  const OpCode EXPRESSION_OPCODE4[] = {
      OP_FALSE, OP_FALSE, OP_EQUAL, OP_NOT, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT4[] = {};

  expressionAssert(EXPRESSION4, EXPRESSION_OPCODE4, EXPRESSION_CONSTANT4);

  const char *EXPRESSION5 = "!(5 - 4 > 3 * 2 == !nil)";
  const OpCode EXPRESSION_OPCODE5[] = {
      OP_CONSTANT, 0,      OP_CONSTANT, 1,        OP_SUBSTRACT,
      OP_CONSTANT, 2,      OP_CONSTANT, 3,        OP_MULTIPLY,
      OP_GREATER,  OP_NIL, OP_NOT,      OP_EQUAL, OP_NOT,
      OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT5[] = {
      NUMBER_VAL(5),
      NUMBER_VAL(4),
      NUMBER_VAL(3),
      NUMBER_VAL(2),
  };
  expressionAssert(EXPRESSION5, EXPRESSION_OPCODE5, EXPRESSION_CONSTANT5);

  greenPrint("\nExpression parser is working\n");
}
int main(int argc, char *argV[]) {
  // Setting env variable
  // Basic ASSERTions for testing
  if (argc != 2) {
    tokenTest();
    expressionCompilerTest();
    vmTest();
    return 0;
  }
  char *arg = argV[1];
  if (strcmp(arg, "token") == 0) {
    tokenTest();
  } else if (strcmp(arg, "compiler") == 0) {
    expressionCompilerTest();
  } else if (strcmp(arg, "vm") == 0) {
    vmTest();
  } else {
    failedExit("Typing error in argument");
  }
  // Evaluating VM,If it works
  return 0;
}
