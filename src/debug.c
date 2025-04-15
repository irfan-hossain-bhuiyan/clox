#include "debug.h"
#include "chunk.h"
#include "common.h"
#include "object.h"
#include "value.h"
#include <scanner.h>
#include <stdint.h>
#include <stdio.h>
void disassembleChunk(const Chunk *chunk, const char *name) {
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
static void ppToken(Token token) { // pp for pretty Print
  printf("%.*s <type: %s ,line: %d> ,", token.length, token.start,
         tokenTypeToString(token.type), token.line);
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
static int constantInstruction(const char *name, const Chunk *chunk,
                               int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}
int disassembleInstruction(const Chunk *chunk, int offset) {
  printf("%4d", offset);
  if (offset > 0 &&
      chunk->lines[offset] ==
          chunk->lines[offset - 1]) { // It is a way to check the code has gone
                                      // to new line or not
    printf("    |");
  } else {
    printf("%4d ", chunk->lines[offset]);
  }
  OpCode instruction = byteToOpCode(chunk->code[offset]);
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
    return simpleInstruction("OP_SUBSTRACT", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_GREATER:
    return simpleInstruction("OP_GREATER", offset);
  case OP_LESS:
    return simpleInstruction("OP_LESS", offset);
  case OP_EQUAL:
    return simpleInstruction("OP_EQUAL", offset);
  case OP_FALSE:
    return simpleInstruction("OP_FALSE", offset);
  case OP_TRUE:
    return simpleInstruction("OP_TRUE", offset);
  case OP_NIL:
    return simpleInstruction("OP_FALSE", offset);
  case OP_NOT:
    return simpleInstruction("OP_NOT", offset);
  case OP_PRINT:
    return simpleInstruction("OP_PRINT", offset);
  case OP_POP:
    return simpleInstruction("OP_POP", offset);
  case OP_DEFINE_GLOBAL:
    return constantInstruction("OP_DEFINE_GLOBAL", chunk, offset);
  case OP_EOE:
    return simpleInstruction("OP_EOE", offset);
  case OP_GET_GLOBAL:
    return constantInstruction("OP_GET_GLOBAL", chunk,offset);
    break;
  }
}
const char *opcodeToString(OpCode instruction) {
  switch (instruction) {
  case OP_RETURN:
    return "OP RETURN";
  case OP_CONSTANT:
    return "OP Constant";
  case OP_NEGATE:
    return "OP_NEGATE";
  case OP_ADD:
    return "OP_ADD";
  case OP_SUBSTRACT:
    return "OP_SUBSTRACT";
  case OP_DIVIDE:
    return "OP_DIVIDE";
  case OP_MULTIPLY:
    return "OP_MULTIPLY";
  case OP_NIL:
    return "OP_NIL";
  case OP_TRUE:
    return "OP_TRUE";
  case OP_FALSE:
    return "OP_FALSE";
  case OP_EQUAL:
    return "OP_EQUAL";
  case OP_GREATER:
    return "OP_GREATER";
  case OP_LESS:
    return "OP_LESS";
  case OP_NOT:
    return "OP_NOT";
  case OP_PRINT:
    return "OP_PRINT";
  case OP_POP:
    return "OP_POP";
  case OP_DEFINE_GLOBAL:
    return "OP_DEFINE_GLOBAL";
  case OP_EOE:
    return "OP_EOE";
  case OP_GET_GLOBAL:
    return "OP_GET_GLOBAL";
    break;
  }
}

Str32 valueToString(Value value) {
  Str32 output;
  switch (value.type) {

  case VAL_BOOL:
    sprintf(output.core, "%s", value.as.boolean ? "true" : "false");
    break;
  case VAL_NIL:
    sprintf(output.core, "nil");
    break;
  case VAL_NUMBER:
    sprintf(output.core, "%f", value.as.number);
    break;
  case VAL_OBJ:
    objectToString(output.core, value);
  }
  return output;
}
void disassembleTable(const Table *table) {
  for (int i = 0; i < table->capacity; i++) {
    printf("%4d", i);
    Entry *entry = &table->entries[i];
    bool isEmpty = entry->key == NULL;
    if (isEmpty)
      printf("----------------------\n");
    else {
      ObjString *nonNullKey = entry->key;
      printf("%16s (%X): %s\n", nonNullKey->chars, nonNullKey->hash,
             valueToString(entry->value).core);
    }
  }
}

#include <unistd.h>  // For pipe(), dup(), dup2(), read(), close()
#include <stdio.h>   // For stdout, perror()
#include <stdlib.h>  // For exit()

// Static variables to maintain state between function calls
static int pipe_read_fd;       // Reading end of the pipe
static int original_stdout;    // Saved original stdout file descriptor
static char *capture_buffer;   // Pointer to the user-provided buffer
static size_t capture_size;    // Size of the buffer

void beginScanPrint(char *buffer, size_t size) {
    // Store the buffer and its size
    capture_buffer = buffer;
    capture_size = size;

    // Create a pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }
    pipe_read_fd = pipefd[0];  // Reading end of the pipe

    // Save the current stdout
    original_stdout = dup(STDOUT_FILENO);
    if (original_stdout == -1) {
        perror("dup failed");
        exit(1);
    }

    // Redirect stdout to the writing end of the pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2 failed");
        exit(1);
    }

    // Close the writing end since stdout now uses it
    close(pipefd[1]);
}

void endScanPrint(void) {
    // Restore the original stdout
    if (dup2(original_stdout, STDOUT_FILENO) == -1) {
        perror("dup2 restore failed");
        exit(1);
    }
    close(original_stdout);

    // Read from the pipe into the buffer, leaving space for null terminator
    ssize_t bytes_read = read(pipe_read_fd, capture_buffer, capture_size - 1);
    if (bytes_read > 0) {
        capture_buffer[bytes_read] = '\0';  // Null-terminate the string
    } else {
        capture_buffer[0] = '\0';  // If no data or error, empty string
    }

    // Clean up by closing the reading end of the pipe
    close(pipe_read_fd);
}
