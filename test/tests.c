#include "chunk.h"
#include "debug.h"
#include <assert.h>
#include <compiler.h>
#include <scanner.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
bool RUNNING_WITH_CTEST;
// A simple function to test; alternatively, include your header file if this
// function is declared there.
const char *EXAMPLE_CODE = "var a=10;\n"
                           "var b=15;\n";
const char *EXAMPLE_EXPRESSION = "12+15*3--12";
const OpCode EXAMPLE_EXPRESSION_OPCODE[] = {
    OP_CONSTANT, 0,      OP_CONSTANT, 1, OP_CONSTANT, 2,
    OP_MULTIPLY, OP_ADD, OP_CONSTANT, 3, OP_NEGATE,   OP_SUBSTRACT,
};
const double EXPRESSION_CONSTANT[] = {12, 15, 3, 12};
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

void expressionAssert(void) {
  Chunk chunk;
  initChunk(&chunk);
  compile(EXAMPLE_EXPRESSION, &chunk);
  for (int i = 0; i < chunk.count; i++) {
    OpCode generatedInstruction = chunk.code[i];
    OpCode expectedInstruction = EXAMPLE_EXPRESSION_OPCODE[i];
    fprintf(stdout, " Generated: opcode:%s \n",
            opcodeToString(generatedInstruction));
    fprintf(stdout, " Expected:  opcode:%s \n",
            opcodeToString(expectedInstruction));

    if (RUNNING_WITH_CTEST) {
      if (chunk.code[i] != EXAMPLE_EXPRESSION_OPCODE[i]) {
        fprintf(stderr, "Error at opcode index %d\n", i);
        exit(EXIT_FAILURE);
      }
    }
  }
  for (int i = 0; i < chunk.constants.count; i++) {
    Value generatedConstant = chunk.constants.values[i];
    Value expectedConstant = EXPRESSION_CONSTANT[i];

    if (RUNNING_WITH_CTEST) {
      if (generatedConstant != expectedConstant) {
        fprintf(stderr, "Error at opcode index %d\n", i);
        exit(EXIT_FAILURE);
      }
    }
  }
}

int main(void) {
  // Setting env variable
  const char *ctest_env = getenv("RUNNING_UNDER_CTEST");
  RUNNING_WITH_CTEST = (ctest_env && strcmp(ctest_env, "1") == 0);
  // Basic assertions for testing
  tokenAssert();
  printf("Tokens is working.");
  expressionAssert();
  printf("Expression parser is working");
  return 0;
}
