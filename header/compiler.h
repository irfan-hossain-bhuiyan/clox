#ifndef clox_compiler_h
#define clox_compiler_h
#include "vm.h"
bool evaluate(const char* source,Chunk* chunk); //The chunk pointer also set currentChunk to the input
#endif
