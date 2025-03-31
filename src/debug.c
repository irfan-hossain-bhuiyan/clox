#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <scanner.h>
#include <stdint.h>
#include <stdio.h>
void disassembleChunk(Chunk *chunk, const char *name) {
  printf("--> %s <--\n", name);
  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}
// It prints out the token that has been given to scanner.
const char *tokenTypeToString(TokenType tt) {
  switch (tt) {
  case TOKEN_LEFT_PRACE:
    return "TOKEN_LEFT_PRACE";
  case TOKEN_RIGHT_PRACE:
    return "TOKEN_RIGHT_PRACE";
  case TOKEN_LEFT_BRACE:
    return "TOKEN_LEFT_BRACE";
  case TOKEN_RIGHT_BRACE:
    return "TOKEN_RIGHT_BRACE";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_DOT:
    return "TOKEN_DOT";
  case TOKEN_MINUS:
    return "TOKEN_MINUS";
  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_SLASH:
    return "TOKEN_SLASH";
  case TOKEN_STAR:
    return "TOKEN_STAR";
  case TOKEN_BANG:
    return "TOKEN_BANG";
  case TOKEN_BANG_EQUAL:
    return "TOKEN_BANG_EQUAL";
  case TOKEN_EQUAL:
    return "TOKEN_EQUAL";
  case TOKEN_EQUAL_EQUAL:
    return "TOKEN_EQUAL_EQUAL";
  case TOKEN_GREATER:
    return "TOKEN_GREATER";
  case TOKEN_GREATER_EQUAL:
    return "TOKEN_GREATER_EQUAL";
  case TOKEN_LESS:
    return "TOKEN_LESS";
  case TOKEN_LESS_EQUAL:
    return "TOKEN_LESS_EQUAL";
  case TOKEN_IDENTIFIER:
    return "TOKEN_IDENTIFIER";
  case TOKEN_STRING:
    return "TOKEN_STRING";
  case TOKEN_NUMBER:
    return "TOKEN_NUMBER";
  case TOKEN_AND:
    return "TOKEN_AND";
  case TOKEN_OR:
    return "TOKEN_OR";
  case TOKEN_NOT:
    return "TOKEN_NOT";
  case TOKEN_TRUE:
    return "TOKEN_TRUE";
  case TOKEN_FALSE:
    return "TOKEN_FALSE";
  case TOKEN_NIL:
    return "TOKEN_NIL";
  case TOKEN_IF:
    return "TOKEN_IF";
  case TOKEN_ELSE:
    return "TOKEN_ELSE";
  case TOKEN_FOR:
    return "TOKEN_FOR";
  case TOKEN_WHILE:
    return "TOKEN_WHILE";
  case TOKEN_VAR:
    return "TOKEN_VAR";
  case TOKEN_CLASS:
    return "TOKEN_CLASS";
  case TOKEN_THIS:
    return "TOKEN_THIS";
  case TOKEN_SUPER:
    return "TOKEN_SUPER";
  case TOKEN_FUN:
    return "TOKEN_FUN";
  case TOKEN_PRINT:
    return "TOKEN_PRINT";
  case TOKEN_RETURN:
    return "TOKEN_RETURN";
  case TOKEN_ERROR:
    return "TOKEN_ERROR";
  case TOKEN_EOF:
    return "TOKEN_EOF";
  }
}
static void ppToken(Token token) {//pp for pretty Print
  printf("%.*s <type: %s ,line: %d> ,", token.length, token.start, tokenTypeToString(token.type),token.line);
}

void disassembleToken(void) {
  Token t;
  do {
    t = scanToken();
    ppToken(t);
  } while (t.type != TOKEN_EOF);
}

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}
static int constantInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}
int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%4d", offset);
  if (offset > 0 &&
      chunk->lines[offset] ==
          chunk->lines[offset - 1]) { // It is a way to check the code has gone
                                      // to new line or not
    printf("    |");
  } else {
    printf("%4d ", chunk->lines[offset]);
  }
  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_RETURN:
    return simpleInstruction("OP RETURN", offset);
  case OP_CONSTANT:
    return constantInstruction("OP Constant", chunk, offset);
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBSTRACT:
    return simpleInstruction("OP_SUBSTRACT",offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE",offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY",offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
}
