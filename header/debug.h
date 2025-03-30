#ifndef clox_debug_h
#define clox_debug_h
#include "chunk.h"

void disassembleChunk(Chunk* chunk,const char* name);
void disassembleToken(void);
int disassembleInstruction(Chunk* chunk,int offset);
#endif
