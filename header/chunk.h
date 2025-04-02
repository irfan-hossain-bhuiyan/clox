#ifndef clox_chunk_h
#define clox_chunk_h
#include "common.h"
#include "value.h"
#include <stdint.h>
typedef enum{
	OP_RETURN,
	OP_CONSTANT,
	OP_NIL,
	OP_TRUE,
	OP_FALSE,
	OP_EQUAL,
	OP_GREATER,
	OP_LESS,
	OP_NOT,
	OP_NEGATE,
	OP_ADD,
	OP_SUBSTRACT,
	OP_DIVIDE,
	OP_MULTIPLY,
} OpCode; // OpCode stands for operation code.
// The thing is I would make an another struct for code and lines,
// because the size of them are equivalent.
// I think the author didn't do it for the readibility.
typedef struct{
int count;
int capacity;
uint8_t* code;//Code is opcode.This is a vector.
	      //This operation pointer also includes pointer to constants.
	      //which are in valuearray.
int* lines;//This is a vector.
ValueArray constants;
} Chunk; // Chunk has 2 vector,One is opcode which is instruction 
	 // and lines.Line number of the instruction.
void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk,uint8_t byte,int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk,Value value);
#endif
