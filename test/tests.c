#include "chunk.h"
#include "common.h"
#include "debug.h"
#include <assert.h>
#include <compiler.h>
#include <scanner.h>
#include <stdio.h>
#include <stdlib.h>
// A simple function to test; alternatively, include your header file if this
// function is declared there.
const char *EXAMPLE_CODE = "var a=10;\n"
                           "var b=15;\n";
const char *EXAMPLE_EXPRESSION= "12+15*3--12";
const OpCode EXPRESSION_OPCODE={

};
typedef struct {
  TokenType type;
  int line;
} DToken;
const DToken EXAMPLE_TOKEN[] = {
    {TOKEN_VAR, 0},    {TOKEN_IDENTIFIER, 0}, {TOKEN_EQUAL, 0},
    {TOKEN_NUMBER, 0}, {TOKEN_SEMICOLON, 0},

    {TOKEN_VAR, 1},    {TOKEN_IDENTIFIER, 1}, {TOKEN_EQUAL, 1},
    {TOKEN_NUMBER, 1}, {TOKEN_SEMICOLON, 1},

};

void tokenAssert(void) {
  initScanner(EXAMPLE_CODE);
  int i = 0;
  while (true) {
    Token generatedToken = scanToken();
    DToken expectedToken = EXAMPLE_TOKEN[i];
    fprintf(stdout, "  Input: (line: %d, type: %s)\n", expectedToken.line,
              tokenTypeToString(expectedToken.type));
    fprintf(stdout, "  Output: (line: %d, type: %s)\n", generatedToken.line,
              tokenTypeToString(generatedToken.type));

    if (generatedToken.line != expectedToken.line ||
        generatedToken.type != expectedToken.type) {
      fprintf(stderr, "Error at token index %d:\n", i);
      exit(EXIT_FAILURE); // Causes the test to fail in CTest
    }
    i++;
  }
}
int main(void) {
  // Basic assertions for testing
  tokenAssert();
  printf("Tokens is working perfectly.");
  return 0;
}
