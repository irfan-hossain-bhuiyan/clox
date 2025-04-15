#ifndef clox_debug_h
#define clox_debug_h
#include "chunk.h"
#include "scanner.h"
#include "table.h"
#include <customType.h>
const char *tokenTypeToString(TokenType tt);
const char *opcodeToString(OpCode opcode);

Str32 valueToString(Value value);

void disassembleChunk(const Chunk *chunk, const char *name);
void disassembleToken(void);
int disassembleInstruction(const Chunk *chunk, int offset);

void disassembleTable(const Table* table);


void beginScanPrint(char *buffer, size_t size);
void endScanPrint(void);
#endif

