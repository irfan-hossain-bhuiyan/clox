
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"
#include <__stdarg_va_list.h>
#include <assert.h>
#include <compiler.h>
#include <logging.h>
#include <object.h>
#include <scanner.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <table.h>
static bool haltOnFailure = true;
static void failedExit(const char *errorReport, ...) {
  va_list args;
  va_start(args, errorReport);
  fprintf(stderr, errorReport, args);
  va_end(args);
  if (haltOnFailure) {
    exit(EXIT_FAILURE);
  }
}
#define ASSERT(x)                                                              \
  do {                                                                         \
    if (!(x)) {                                                                \
      failedExit("ASSERT(" #x ")");                                            \
    }                                                                          \
  } while (false);

static Value STRING_VAL(const char *chars) {
  return OBJ_VAL(copyString(chars, strlen(chars)));
}

// A simple function to test; alternatively, include your header file if this
// function is declared there.
typedef struct {
  TokenType type;
  int line;
} DToken;

void tokenAssert(const char *str, const DToken *tokens) {
  bluePrint("Code is :%s", str);
  initScanner(str);
  int i = 0;
  while (true) {
    Token generatedToken = scanToken();
    DToken expectedToken = tokens[i];
    fprintf(stdout, "  Expected: (line: %d, type: %s)\n", expectedToken.line,
            tokenTypeToString(expectedToken.type));
    fprintf(stdout, "  Generated: (line: %d, type: %s , text: %.*s)\n",
            generatedToken.line, tokenTypeToString(generatedToken.type),
            generatedToken.length, generatedToken.start);

    if (generatedToken.line != expectedToken.line ||
        generatedToken.type != expectedToken.type) {
      failedExit("Error at token index %d:\n",
                 i); // Causes the test to fail in CTest
    }
    if (generatedToken.type == TOKEN_EOF) {
      break;
    }
    i++;
  }
}

void expressionAssert(const char *expression, const OpCode *expressionOpcodes,
                      const Value *expressionConstants) {
  Chunk chunk;
  initChunk(&chunk);
  bool allOk = compile(expression, &chunk);
  bluePrint("\nCode: %s\n", expression);
  if (!allOk) {
    failedExit("Trying to compile expression that are not valid.");
  }
  for (int i = 0; i < chunk.count; i++) {
    OpCode generatedInstruction = chunk.code[i];
    OpCode expectedInstruction = expressionOpcodes[i];
    fprintf(stdout, " Generated: opcode:%s \n",
            opcodeToString(generatedInstruction));
    fprintf(stdout, " Expected:  opcode:%s \n",
            opcodeToString(expectedInstruction));
    if (chunk.code[i] != expressionOpcodes[i]) {
      failedExit("Error at opcode index %d\n", i);
    }

    // Different printing if the next code is constant pointer
    if (generatedInstruction == OP_CONSTANT) {
      i++;
      generatedInstruction = chunk.code[i];
      expectedInstruction = expressionOpcodes[i];
      Value generatedConstant = chunk.constants.values[generatedInstruction];
      Value expectedConstant = expressionConstants[expectedInstruction];
      fprintf(stdout, " Generated: ConstantPointer:%d --> %s\n",
              generatedInstruction, valueToString(generatedConstant).core);
      fprintf(stdout, " Expected:  ConstantPointer:%d --> %s\n",
              expectedInstruction, valueToString(expectedConstant).core);
      if (chunk.code[i] != expressionOpcodes[i]) {
        failedExit("Constant pointer mismatch %d\n", i);
      }
      if (!valuesEqual(generatedConstant, expectedConstant)) {
        failedExit("Constant mismatched: %d\n", i);
      }
    }
  }

  // Checking for constant
  freeChunk(&chunk);
}
void exprEvalAssert(const char *expr, Value expectedOutput) {

  initVM();
  printf("\n\nThe expression: \"%s\"\n", expr);
  InterpretResult result = interpret(expr);
  switch (result) {

  case INTERPRET_OK:
    break;
  case INTERPRET_COMPILE_ERROR:
    failedExit("tests outputs compile error.");
  case INTERPRET_RUNTIME_ERROR:
    failedExit("tests outputs have runtime error.");
    break;
  }
  Value returnedOutput = getLastReturn();
  ASSERT(valuesEqual(returnedOutput, expectedOutput));
  freeVM();
}
void exprErrorAssert(const char *expr, InterpretResult error) {
  initVM();
  printf(TO_BLUE("Code is->\"%s\"\n"), expr);
  InterpretResult result = interpret(expr);
  ASSERT(error == result);
  printf("All ok\n");
  freeVM();
}

void stringHashTest(void) {
  ObjString *str1 = takeString("Hello", 5);
  ObjString *str2 = takeString("World", 5);
  ObjString *str3 = takeString("Hello", 5);
  ObjString *str4 = takeString("hello", 5);
  assert(str1->hash == str3->hash);
  assert(str1->hash != str4->hash);
  assert(str2->hash != str3->hash);
}

void tableTest(void) {
  // Initialize table for primary tests.
  Table table;
  initTable(&table);

  // --- Test 1: Basic insertion and retrieval ---
  // Insert several key-value pairs with mixed value types.
  tableSet(&table, takeString(" i", 2), NUMBER_VAL(12.0));
  tableSet(&table, takeString("OK", 2), BOOL_VAL(true));
  tableSet(&table, takeString("3", 1), STRING_VAL("Will be deleted"));

  Value value = NIL_VAL;
  bluePrint("Table get check");
  // Check a key that does not exist (note: key lookup is case-sensitive).
  assert(!tableGet(&table, takeString("ok", 2), &value));
  assert(IS_NIL(value));

  // Retrieve the 'OK' key and verify its boolean value.
  assert(tableGet(&table, takeString("OK", 2), &value));
  assert(valuesEqual(value, BOOL_VAL(true)));
  bluePrint("Table get works");

  // --- Test 2: Deletion ---
  bluePrint("Table delete checks");
  // Delete a key that exists.
  assert(tableDelete(&table, takeString("3", 1)));
  // Attempt to delete a key that does not exist.
  assert(!tableDelete(&table, takeString("2", 1)));
  // Confirm that the deleted key no longer exists.
  assert(!tableGet(&table, takeString("3", 1), &value));
  // Ensure that the previous value remains unchanged (the deleted value is not
  // overwritten in 'value'). In this test, 'value' is expected to still hold
  // the old value from the previous successful get. Depending on your
  // implementation, this behavior might need adjustment.
  assert(valuesEqual(value, BOOL_VAL(true)));

  // --- Test 3: Key replacement ---
  // Replace an existing key's value.
  assert(tableSet(&table, takeString("OK", 2), NUMBER_VAL(42.0)));
  // Retrieve the key to ensure that the value has been replaced.
  assert(tableGet(&table, takeString("OK", 2), &value));
  assert(valuesEqual(value, NUMBER_VAL(42.0)));

  // --- Test 4: Merging tables using tableAddAll ---
  // Create a second table to test merging.
  Table table2;
  initTable(&table2);
  // Insert new keys and also a key that overlaps with an existing key in
  // 'table'.
  tableSet(&table2, takeString("no", 2), STRING_VAL("NO"));
  tableSet(&table2, takeString(" i", 2), STRING_VAL("replaced"));

  // Merge table2 into table.
  tableAddAll(&table2, &table);

  // After merging, verify that:
  // 1. Keys from table2 were added.
  // 2. Existing keys in 'table' (like " i") were replaced by those in table2.

  // Check key " i" (which should have been replaced).
  assert(tableGet(&table, takeString(" i", 2), &value));
  assert(valuesEqual(value, STRING_VAL("replaced")));

  // Check key "no" was added.
  assert(tableGet(&table, takeString("no", 2), &value));
  assert(valuesEqual(value, STRING_VAL("NO")));

  // Clean up both tables.
  freeTable(&table2);
  freeTable(&table);

  bluePrint("All table tests passed successfully.");
}
// void tableTest(void) {
//   Table table;
//   initTable(&table);
//   // Test 1
//   tableSet(&table, takeString(" i", 2), NUMBER_VAL(12.0));
//   tableSet(&table, takeString("OK", 2), BOOL_VAL(true));
//   tableSet(&table, takeString("3", 1), STRING_VAL("Will be deleted"));
//
//   Table table2;
//   initTable(&table2);
//   tableSet(&table2, takeString("no", 2), STRING_VAL("NO"));
//   tableSet(&table2, takeString(" i", 2), STRING_VAL("replaced"));
//
//   Value value = NIL_VAL;
//   bluePrint("Table get check");
//   assert(!tableGet(&table, takeString("ok", 2), &value));
//   assert(IS_NIL(value));
//   assert(tableGet(&table, takeString("OK", 2), &value));
//   assert(valuesEqual(value, BOOL_VAL(true)));
//   bluePrint("Table get works");
//   bluePrint("Table delete checks");
//   assert(tableDelete(&table, takeString("3", 1)));
//   assert(!tableDelete(&table, takeString("2", 1)));
//   assert(!tableGet(&table, takeString("3", 1), &value));
//   assert(valuesEqual(value, BOOL_VAL(true)));
//
//
//   tableAddAll(&table, &table2);
//   freeTable(&table);
// }

void vmTest(void) {
  exprEvalAssert("2+3*(3*3)", NUMBER_VAL(29));
  exprEvalAssert("2+2+2*0", NUMBER_VAL(4));
  exprEvalAssert("2<3.1", BOOL_VAL(true));
  exprEvalAssert("2>3.1", BOOL_VAL(false));
  exprEvalAssert("2>=3.1", BOOL_VAL(false));
  exprEvalAssert("2<=3.1", BOOL_VAL(true));
  exprEvalAssert("2*3<=3.1415*2", BOOL_VAL(true));
  exprEvalAssert("false!=false", BOOL_VAL(false));
  exprEvalAssert("false==false", BOOL_VAL(true));
  exprEvalAssert("nil==nil", BOOL_VAL(true));
  exprEvalAssert("3.14159", NUMBER_VAL(3.14159));
  exprEvalAssert("true", BOOL_VAL(true));
  exprEvalAssert("1+--3", NUMBER_VAL(4));
  exprEvalAssert("\"I am irfan\"", STRING_VAL("I am irfan"));
  exprEvalAssert("\"I am irfan.\"+ \" I am a student.\"",
                 STRING_VAL("I am irfan. I am a student."));
  exprEvalAssert("\"hello\"+\" \"+\"world\"==\"hello world\"", BOOL_VAL(true));
  exprErrorAssert("1++2--3", INTERPRET_COMPILE_ERROR);
  exprErrorAssert("true*3+2", INTERPRET_RUNTIME_ERROR);
  exprErrorAssert("true+true", INTERPRET_RUNTIME_ERROR);
  exprErrorAssert("nil<=nil", INTERPRET_RUNTIME_ERROR);
  exprErrorAssert("\"irfan\">\"everyone else\"", INTERPRET_RUNTIME_ERROR);

  greenPrint("\n VM is working \n");
}

static void tokenTest(void) {
  const char *CODE = "var a=10;\n"
                     "var b=15;\n";
  const DToken TOKENS[] = {
      {TOKEN_VAR, 1},    {TOKEN_IDENTIFIER, 1}, {TOKEN_EQUAL, 1},
      {TOKEN_NUMBER, 1}, {TOKEN_SEMICOLON, 1},

      {TOKEN_VAR, 2},    {TOKEN_IDENTIFIER, 2}, {TOKEN_EQUAL, 2},
      {TOKEN_NUMBER, 2}, {TOKEN_SEMICOLON, 2},  {TOKEN_EOF, 3},
  };

  tokenAssert(CODE, TOKENS);

  const char *CODE1 = "a>b";
  const DToken TOKENS1[] = {{TOKEN_IDENTIFIER, 1},
                            {TOKEN_GREATER, 1},
                            {TOKEN_IDENTIFIER, 1},
                            {TOKEN_EOF, 1}};

  tokenAssert(CODE1, TOKENS1);
  const char *CODE2 = "a>=b";
  const DToken TOKENS2[] = {{TOKEN_IDENTIFIER, 1},
                            {TOKEN_GREATER_EQUAL, 1},
                            {TOKEN_IDENTIFIER, 1},
                            {TOKEN_EOF, 1}};
  tokenAssert(CODE2, TOKENS2);

  const char *CODE3 = "a>b==\"true\"\n\n;";
  const DToken TOKENS3[] = {
      {TOKEN_IDENTIFIER, 1},  {TOKEN_GREATER, 1}, {TOKEN_IDENTIFIER, 1},
      {TOKEN_EQUAL_EQUAL, 1}, {TOKEN_STRING, 1},  {TOKEN_SEMICOLON, 3},
      {TOKEN_EOF, 3},
  };
  tokenAssert(CODE3, TOKENS3);

  const char *CODE4 = "false!=false";
  const DToken TOKENS4[] = {{TOKEN_FALSE, 1},
                            {TOKEN_BANG_EQUAL, 1},
                            {TOKEN_FALSE, 1},
                            {TOKEN_EOF, 1}};

  tokenAssert(CODE4, TOKENS4);

  const char *CODE5 = "2<3";
  const DToken TOKENS5[] = {
      {TOKEN_NUMBER, 1}, {TOKEN_LESS, 1}, {TOKEN_NUMBER, 1}, {TOKEN_EOF, 1}};
  tokenAssert(CODE5, TOKENS5);
  greenPrint("\nTokens is working.\n");
}
static void expressionCompilerTest(void) {
  const char *EXPRESSION0 = "12+15*3--12";
  const OpCode EXPRESSION_OPCODE0[] = {
      OP_CONSTANT, 0,      OP_CONSTANT, 1, OP_CONSTANT, 2,
      OP_MULTIPLY, OP_ADD, OP_CONSTANT, 3, OP_NEGATE,   OP_SUBSTRACT,
      OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT0[] = {NUMBER_VAL(12), NUMBER_VAL(15),
                                        NUMBER_VAL(3), NUMBER_VAL(12)};

  expressionAssert(EXPRESSION0, EXPRESSION_OPCODE0, EXPRESSION_CONSTANT0);

  bluePrint("\nExperssion 0 passed\n");
  const char *EXPRESSION1 = "2+3*(3*3)";
  const OpCode EXPRESSION_OPCODE1[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1,           OP_CONSTANT, 2,
      OP_CONSTANT, 3, OP_MULTIPLY, OP_MULTIPLY, OP_ADD,      OP_RETURN};
  const Value EXPRESSION_CONSTANT1[] = {NUMBER_VAL(2), NUMBER_VAL(3),
                                        NUMBER_VAL(3), NUMBER_VAL(3)};

  expressionAssert(EXPRESSION1, EXPRESSION_OPCODE1, EXPRESSION_CONSTANT1);
  bluePrint("\nExperssion 1 passed\n");

  const char *EXPRESSION2 = "2<3";
  const OpCode EXPRESSION_OPCODE2[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1, OP_LESS, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT2[] = {
      NUMBER_VAL(2),
      NUMBER_VAL(3),
  };
  expressionAssert(EXPRESSION2, EXPRESSION_OPCODE2, EXPRESSION_CONSTANT2);

  const char *EXPRESSION3 = "3<=3";
  const OpCode EXPRESSION_OPCODE3[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1, OP_GREATER, OP_NOT, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT3[] = {
      NUMBER_VAL(3),
      NUMBER_VAL(3),
  };
  expressionAssert(EXPRESSION3, EXPRESSION_OPCODE3, EXPRESSION_CONSTANT3);

  const char *EXPRESSION4 = "false!=false";
  const OpCode EXPRESSION_OPCODE4[] = {
      OP_FALSE, OP_FALSE, OP_EQUAL, OP_NOT, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT4[] = {};

  expressionAssert(EXPRESSION4, EXPRESSION_OPCODE4, EXPRESSION_CONSTANT4);

  const char *EXPRESSION5 = "!(5 - 4 > 3 * 2 == !nil)";
  const OpCode EXPRESSION_OPCODE5[] = {
      OP_CONSTANT, 0,      OP_CONSTANT, 1,        OP_SUBSTRACT,
      OP_CONSTANT, 2,      OP_CONSTANT, 3,        OP_MULTIPLY,
      OP_GREATER,  OP_NIL, OP_NOT,      OP_EQUAL, OP_NOT,
      OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT5[] = {
      NUMBER_VAL(5),
      NUMBER_VAL(4),
      NUMBER_VAL(3),
      NUMBER_VAL(2),
  };
  expressionAssert(EXPRESSION5, EXPRESSION_OPCODE5, EXPRESSION_CONSTANT5);

  const char *EXPRESSION6 = "\"Hello \"+\"World\"==\"Hello World\" ";
  const OpCode EXPRESSION_OPCODE6[] = {
      OP_CONSTANT, 0, OP_CONSTANT, 1,         OP_ADD,
      OP_CONSTANT, 2, OP_EQUAL,    OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT6[] = {
      STRING_VAL("Hello "), STRING_VAL("World"), STRING_VAL("Hello World")};
  expressionAssert(EXPRESSION6, EXPRESSION_OPCODE6, EXPRESSION_CONSTANT6);

  const char *EXPRESSION7 =
      "\"Hello \"+\"World\" +\"I am irfan\"==\"Hello WorldI am irfan\"";
  const OpCode EXPRESSION_OPCODE7[] = {
      OP_CONSTANT, 0,      OP_CONSTANT, 1, OP_ADD,   OP_CONSTANT,
      2,           OP_ADD, OP_CONSTANT, 3, OP_EQUAL, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT7[] = {
      STRING_VAL("Hello "), STRING_VAL("World"), STRING_VAL("I am irfan"),
      STRING_VAL("Hello WorldI am irfan")};

  expressionAssert(EXPRESSION7, EXPRESSION_OPCODE7, EXPRESSION_CONSTANT7);

  const char *EXPRESSION8 = "2+2+2*0";
  const OpCode EXPRESSION_OPCODE8[] = {
      OP_CONSTANT, 0,           OP_CONSTANT, 1,           OP_ADD, OP_CONSTANT,
      2,           OP_CONSTANT, 3,           OP_MULTIPLY, OP_ADD, OP_RETURN,
  };
  const Value EXPRESSION_CONSTANT8[] = {
      NUMBER_VAL(2),
      NUMBER_VAL(2),
      NUMBER_VAL(2),
      NUMBER_VAL(0),
  };

  expressionAssert(EXPRESSION8, EXPRESSION_OPCODE8, EXPRESSION_CONSTANT8);
  greenPrint("\nExpression parser is working\n");
}
int main(int argc, char *argV[]) {
  // Setting env variable
  // Basic ASSERTions for testing
  if (argc != 2) {
    tokenTest();
    expressionCompilerTest();
    vmTest();
    return 0;
  }
  char *arg = argV[1];
  if (strcmp(arg, "token") == 0) {
    tokenTest();
  } else if (strcmp(arg, "compiler") == 0) {
    expressionCompilerTest();
  } else if (strcmp(arg, "vm") == 0) {
    vmTest();
  } else if (strcmp(arg, "table") == 0) {
    tableTest();
  } else if (strcmp(arg, "strHash") == 0) {
    stringHashTest();
  } else {
    failedExit("Typing error in argument");
  }
  // Evaluating VM,If it works
  return 0;
}
