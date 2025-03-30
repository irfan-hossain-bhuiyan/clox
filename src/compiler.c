#include "chunk.h"
#include "debug.h"
#include "scanner.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TODO:The parser has two token as input.
// In rlox,I used the source as reference.And a Vec<Error> for all error.
typedef struct {
  Token current;
  Token previous;
  bool haderror;
  bool panicmode;
} Parser;
typedef enum{
	PREC_NONE,
	PREC_ASSIGNMENT,// =
	PREC_OR, // or
	PREC_AND,// and
	PREC_EQUALITY,// ==
	PREC_COMPARASION,//<,<=,>=,>
	PREC_TERM,//+ -
	PREC_FACTOR,// * /
	PREC_UNARY,//! -
	PREC_CALL,// . ()
	PREC_PRIMARY,
} Precedece;


Chunk *compilingChunk;
static Chunk *currentChunk(void) { return compilingChunk; }
Parser parser;
static void errorAt(Token *token, const char *messsage) {
  if (parser.panicmode)
    return; // TODO:I should have error in here.When it is in panic mode it
            // shouldn't call errorAt
  parser.panicmode = true;
  fprintf(stderr, "[line %d] Error", token->line);
  if (token->type == TOKEN_EOF) {
    fprintf(stderr, "At error");
  } else if (token->type == TOKEN_ERROR) {
    // It has been printed before.
    // Use comment
  } else {
    fprintf(stderr, "at '%*s'", token->length, token->start);
  }
  fprintf(stderr, ": '%s'", messsage);
  parser.haderror = true;
}
//Why do we need two error report.
static void error(const char *message) { errorAt(&parser.previous, message); }
static void errorAtCurrent(const char *message) {
  errorAt(&parser.current, message);
}
// We keep looping, reading tokens and reporting the errors,
// until we hit a non-error one or reach the end.
// That way, the rest of the parser only sees valid tokens.
// The current and previous token are stored in this struct:

// Advance function steps through token and removes all tokens that have error in that
static void advance(void) {
  parser.previous = parser.current;//parser,current is endptr.
  for (;;) {
    parser.current = scanToken();
    if (parser.current.type != TOKEN_ERROR)
      break;
    errorAtCurrent(parser.current.start);
  }
}
static void consume(TokenType type, const char *message) {
  if (parser.current.type == type) {
    advance();
    return;
  }
  errorAtCurrent(message);
}
// This is to parse the code,and adding instruction in the chunk.
static void emitByte(uint8_t byte) {
  writeChunk(currentChunk(), byte, parser.previous.line);
}



static void emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}
static void emitReturn(void) { emitByte(OP_RETURN); }
static void endCompiler(void) { 
	emitReturn();
#ifdef DEBUG_PRINT_CODE
	if(!parser.haderror){
		disassembleChunk(currentChunk(), "code");
	}
#endif
}


typedef void (*ParseFn)(void);
typedef struct{
	ParseFn prefix;
	ParseFn infix;
	Precedece precedence;
}ParseRule;
static ParseRule* getRule(TokenType);

// parsePrecedence 
static void parsePrecedence(Precedece precedence){
	advance();
	ParseFn prefixRule=getRule(parser.previous.type)->prefix;
	//if (getRule(parser.previous.type)->precedence<precedence){
	//	error("Calling parsePrecedence on something that only have low precedence");
	//	return;
	//}
	if (prefixRule==NULL){
		error("Expected Expression");return;
	}
	prefixRule();
	while (precedence <= getRule(parser.current.type)->precedence) {
		advance();
		ParseFn infixRule=getRule(parser.previous.type)->infix;
		infixRule();
	}

}
static void expression(void){
	parsePrecedence(PREC_ASSIGNMENT);
}

static void grouping(void) {
  expression();
  consume(TOKEN_RIGHT_PRACE, "Expected ')' in expression.");
}
static uint8_t makeConstant(Value value) {
  int constant = addConstant(currentChunk(), value);
  if (constant > UINT8_MAX) {
    error("Too many constant in one chunk");
    return 0;
  }
  return (uint8_t)constant;
}

static void emitConstant(Value val) {
  emitBytes(OP_CONSTANT, makeConstant(val));
}


static void number(void) {
  double value = strtod(parser.previous.start, NULL);//strtod converts str to double value
  emitConstant(value);
}

static void unary(void) {//The consume of - expression came before unary() function call
  TokenType operatorType = parser.previous.type;
  // Compile the operand
  // We need to evaluate the expression first,and
  // than add the operand.
  parsePrecedence(PREC_UNARY);
  switch (operatorType) {
  case TOKEN_MINUS:
    emitByte(OP_NEGATE);
    break;
  default:
    error("Should have a minus in here.");
    break; // Unreachable
  }
}
static void binary(void){
	TokenType operatorType=parser.previous.type;
	ParseRule* rule=getRule(operatorType);
	parsePrecedence((Precedece)(rule->precedence+1));
	switch (operatorType) {
		case TOKEN_PLUS:emitByte(OP_ADD);return;
		case TOKEN_MINUS:emitByte(OP_SUBSTRACT);return;
		case TOKEN_STAR:emitByte(OP_MULTIPLY);return;
		case TOKEN_SLASH:emitByte(OP_DIVIDE);return;
		default:error("This should be unreacheable.binary operation.");return;
	}
}
ParseRule rules[] = {
	[TOKEN_LEFT_PRACE] = {grouping, NULL, PREC_NONE},
	[TOKEN_RIGHT_PRACE] = {NULL, NULL, PREC_NONE},
	[TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
	[TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},[TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
	[TOKEN_DOT] = {NULL, NULL, PREC_NONE},
	[TOKEN_MINUS] = {unary, binary, PREC_TERM},
	[TOKEN_PLUS] = {NULL, binary, PREC_TERM},
	[TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
	[TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
	[TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
	[TOKEN_BANG] = {NULL, NULL, PREC_NONE},
	[TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
	[TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
	[TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
	[TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
	[TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
	[TOKEN_LESS] = {NULL, NULL, PREC_NONE},
	[TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
	[TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
	[TOKEN_STRING] = {NULL, NULL, PREC_NONE},
	[TOKEN_NUMBER] = {number, NULL, PREC_NONE},
	[TOKEN_AND] = {NULL, NULL, PREC_NONE},
	[TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
	[TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
	[TOKEN_FALSE] = {NULL, NULL, PREC_NONE},
	[TOKEN_FOR] = {NULL, NULL, PREC_NONE},
	[TOKEN_FUN] = {NULL, NULL, PREC_NONE},
	[TOKEN_IF] = {NULL, NULL, PREC_NONE},
	[TOKEN_NIL] = {NULL, NULL, PREC_NONE},
	[TOKEN_OR] = {NULL, NULL, PREC_NONE},
	[TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
	[TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
	[TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
	[TOKEN_THIS] = {NULL, NULL, PREC_NONE},
	[TOKEN_TRUE] = {NULL, NULL, PREC_NONE},
	[TOKEN_VAR] = {NULL, NULL, PREC_NONE},
	[TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
	[TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
	[TOKEN_EOF] = {NULL, NULL, PREC_NONE}, 
};

static ParseRule* getRule(TokenType type){
	return &rules[type];
}


//Returns true if there is no error
bool compile(const char *source, Chunk *chunk) {
  initScanner(source);
  compilingChunk = chunk;
  parser.haderror = false;
  parser.panicmode = false;
  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");//It checks if the current token is givenToken,else return error
  endCompiler();
  return !parser.haderror;
  //	int line=-1;
  //	for(;;){
  //		Token token=scanToken();
  //		if(token.line!=line){
  //			printf("%4d",token.line);
  //			line=token.line;
  //		}
  //		else{
  //			printf("  |");
  //		}
  //		printf("%2d '%.*s'\n",token.type,token.length,token.start);
  //		if (token.type==TOKEN_EOF){break;}
  //	}
}
