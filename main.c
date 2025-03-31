#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "scanner.h"
#include "vm.h"
#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//First I create a common.h file,that has all the import
//Created chunk that is a vec of opcode(uint_8) here,
//It also has Value in it for code,So like representing let a=10

//The ValueArray is a vec of values
//
//So chunk is not just vec of opcode,it also contain Vec of values
//memory.h has all the sequential memory allocation func
//it has FREE_ARRAY and GROW_ARRAY
//Disassemble Chunk and Disassemble Instruction for 
//Always thinks for returning value
//Representing Value as Double
//Saving the Value using ValueArray named constants
//As ValueArray is in Chunk,a new addConstant function is added
//OP_code has OP_constant,which not only keep itself in chunk,It also keep
//the constant pointer next to it.
//
//
//Chunk also has vec for line number,That is sequentially with opcode
//In debug,Their is dissambleChunk that print all the instruction string sequentially,using print
//dissambleInstruction 

//VM contains a list of Chunks
//Vm is a global variable,That is passed around to everybody.
//The default function is Interpret,(it both compile and run)
//
//compile function take two argument (source code and a chunk ref) for putting the source code,starts with:
//1. scanner
//2. It also has parser.Parser has previous and current token to have range of token
//2. compile is in compiler.c and .h file
//3. Like scanner Parser current type is the one that had not completed.
//
//hadError to check if a error occurs,and panicMode, to skip piece of token,until a safe zone.
//the run function just initialize the vm
//run function runs the opcode the opcode
//for OP_CONSTANT() it also reads and push the constant
//for OP_NEGATE,it pop the saved element and then pushes it again,changing it to negative
//
//
//VM hold the chunk and ip,the code and instruction pointer.
//ip always hold to the next operation to be executed to.So first deref and than add 1
//always add a debug tracker,for big thing
//VM also contains stack (stack has a fixed sized array of double and a pointer to top value)
//
//Just like before StackTop pointer points to the empty element before add,
//So by setting it in initial position,We got an empty stack
//
//endCompiler calls after the end of the compile function,For now it just add return Opcode
//
//In java when parsing expression,the operator has precendence,In order to parse them,We took a class will parse 
//everything that has higher precedence to it,So like a+-b,Now unary has a higher precendence than +,So 
//Add class will have something like{unary()+unary()}
//
//In c,We don't have idea like abstract class,In here,we will use enum to define precendece in here.
//
// static in c is equivalent to private variable.
// Different files can have static variable and function with same name.
// they won't create linking error.
static void repl(void) {
  char line[1024];
  for (;;) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {// All functions that related to files annotated
					    // f in the function name.
      printf("\n");
      break;
    }
    interpret(line);
  }
}
//This function read from a file path and returns the buufer as a pointer.
static char* readFile(const char* path){ //This one is to just read the file,not do anything else.
	FILE* file=fopen(path,"rb");
	if(file==NULL){
		fprintf(stderr,"Couldn't open file \"%s\".\n",path);
		exit(74);
	}
	fseek(file, 0,SEEK_END);//fseek go to end of the file to find the length of the file.
	size_t fileSize=ftell(file);//ftell get the current position of the file,which is of of the file.
	rewind(file);// rewind returns back from the file.
	char* buffer=(char*)malloc(fileSize+1);// To copy content of the file,I need a buffer same size of
					       //  the file.One extra space for the '\0' character.
	if(buffer==NULL){
		fprintf(stderr,"Not enough space.");
		exit(74);
	}
	size_t bytesRead=fread(buffer,sizeof(char),fileSize,file);
	if(bytesRead<fileSize){
		fprintf(stderr,"Couldn't read file \"%s\".\n",path);
		exit(74);
	}
	buffer[bytesRead]='\0';
	fclose(file);
	return buffer;

}
static void runFile(const char* path){
	char* source=readFile(path);
	InterpretResult result=interpret(source);
	free(source);
	if(result==INTERPRET_COMPILE_ERROR)exit(65);
	if(result==INTERPRET_RUNTIME_ERROR)exit(70);
}
const char * EXAMPLE_CODE="var a=10;\n";
#define MAIN_PROG 0
#define TOKEN_DEBUG 1
#define CHUNK_DEBUG 2
#define PROG TOKEN_DEBUG
int main(int argc, const char *argv[]) {
#if PROG==MAIN_PROG
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }
  freeVM();
  return 0;
#elif PROG==TOKEN_DEBUG
 initScanner(EXAMPLE_CODE); 
 disassembleToken();

#elif PROG==CHUNK_DEBUG
 Chunk chunk;
 initChunk(&chunk);
 const char* EXPRESSION="1+2+2*3-2--4\n";
 compile(EXPRESSION,&chunk);
#ifndef DEBUG_PRINT_CODE
 dissambleChunk(&chunk)
#endif
 freeChunk(&chunk);
#else
  printf("The Prog marco is initialized wrongly")
#endif
  // Chunk chunk;
  // initVM();
  // initChunk(&chunk);
  // int constant = addConstant(&chunk, 1.2);
  // writeChunk(&chunk, OP_CONSTANT, 123);
  // writeChunk(&chunk, constant, 123);
  // constant = addConstant(&chunk, 3.4);
  // writeChunk(&chunk, OP_CONSTANT, 123);
  // writeChunk(&chunk, constant, 123);
  // writeChunk(&chunk, OP_ADD, 123);
  // constant = addConstant(&chunk, 5.6);
  // writeChunk(&chunk, OP_CONSTANT, 123);
  // writeChunk(&chunk, constant, 123);
  // writeChunk(&chunk, OP_DIVIDE, 123);
  // writeChunk(&chunk, OP_NEGATE, 123);
  // writeChunk(&chunk, OP_RETURN, 123);
  // interpret(&chunk);
  // freeChunk(&chunk);
  // freeVM();
  // return 0;
}
