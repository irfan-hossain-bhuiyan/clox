#ifndef clox_scanner_h
#define clox_scanner_h
typedef enum {
  // Single-character token
  TOKEN_LEFT_PRACE,
  TOKEN_RIGHT_PRACE,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_MINUS,
  TOKEN_PLUS,
  TOKEN_SEMICOLON,
  TOKEN_SLASH,
  TOKEN_STAR,

  // One or Two character token
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,

      // Literals
      TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,

  // Keywords
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_NOT,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_NIL,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_FOR,
  TOKEN_WHILE,
  TOKEN_VAR,
  TOKEN_CLASS,
  TOKEN_THIS,
  TOKEN_SUPER,
  TOKEN_FUN,

  //Token Function
  TOKEN_PRINT,
  TOKEN_RETURN,

  

  TOKEN_ERROR,
  TOKEN_EOF,
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
} Token;
Token scanToken(void);
void initScanner(const char *source);
#endif
