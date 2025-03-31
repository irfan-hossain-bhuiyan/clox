#ifndef clox_debug_h
#define clox_debug_h
#include "chunk.h"
#include "scanner.h"
const char *tokenTypeToString(TokenType tt);
const char *opcodeToString(OpCode opcode);
void disassembleChunk(Chunk* chunk,const char* name);
void disassembleToken(void);
int disassembleInstruction(Chunk* chunk,int offset);
#endif
