#include "chunk.h"
#include <stdint.h>
#include <stdlib.h>
#include "common.h"
#include "memory.h"
#include "value.h"
void initChunk(Chunk *chunk){
	chunk->count=0;
	chunk->code=NULL;
	chunk->lines=NULL;
	chunk->capacity=0;
	initValueArray(&chunk->constants);
}
void writeChunk(Chunk *chunk, uint8_t byte,int line){
	if(chunk->capacity<chunk->count+1){
		int Oldcapacity=chunk->capacity;
		chunk->capacity=GROW_CAPACITY(Oldcapacity);
		chunk->code=GROW_ARRAY(uint8_t,chunk->code,Oldcapacity,chunk->capacity);
		chunk->lines=GROW_ARRAY(int,chunk->lines,Oldcapacity,chunk->capacity);
	}
	chunk->code[chunk->count]=byte;
	chunk->lines[chunk->count]=line;
	chunk->count++;
}
void freeChunk(Chunk *chunk){
	FREE_ARRAY(uint8_t,chunk->code,chunk->capacity);
	FREE_ARRAY(int,chunk->lines,chunk->capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}
int addConstant(Chunk *chunk, Value value){
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count-1;
}
OpCode byteToOpCode(uint8_t instruction) { 
	if(instruction<OP_EOE){return instruction;}
	sprintf(stderr, "The byte is invalid,It can't be turned to OpCode.");
	exit(EXIT_FAILURE);
}
