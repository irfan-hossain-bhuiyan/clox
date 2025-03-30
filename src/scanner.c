#include "scanner.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
const int KEYWORDS_AMOUNT = 17;
typedef struct {
  const char *start;
  const char *current;
  int line;
} Scanner;
Scanner scanner;
void initScanner(const char *source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}
static bool isAtEnd(void) { return *scanner.current == '\0'; }
static char advance(void) {
  scanner.current++;
  return scanner.current[-1];
}
static bool match(char expected) {
  if (isAtEnd())
    return false;
  if (*scanner.current != expected)
    return false;
  scanner.current++;
  return true;
}
static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  return token;
}
static char peek(void) { return *scanner.current; }
static char peekNext(void) {
  if (isAtEnd())
    return '\0';
  return scanner.current[1];
}
static Token errorToken(const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;

  return token;
}
static Token string(void) {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      scanner.line++;
    advance();
  }
  // for isAtEnd
  if (isAtEnd())
    return errorToken("Unterminated string");
  // The closing quote
  advance();
  return makeToken(TOKEN_STRING);
}
static bool isDigit(char c) { return '0' <= c && c <= '9'; }
static Token number(void) {
  while (isDigit(peek()))
    advance();
  if (peek() == '.' && isDigit(peekNext())) {
    advance();
    while (isDigit(peek()))
      advance();
  }
  return makeToken(TOKEN_NUMBER);
}
static TokenType checkKeyword(int start, int lenght, const char *rest,
                              TokenType token_type) {
  if (scanner.current - scanner.start == start + lenght &&
      memcmp(scanner.start + start, rest, lenght) == 0) {
    return token_type;
  }
  return TOKEN_IDENTIFIER;
}
static bool isAlpha(char c) {
  return ('a'<= c && c <= 'z') || ('A'<=c && c <= 'Z') || c == '_';
}

/*IdentifierType checks if it is a keyword,or literal.
 */
static TokenType identifierType(void) {
  switch (scanner.start[0]) {
  case 'a':
    return checkKeyword(1, 2, "nd", TOKEN_AND);
  case 'c':
    return checkKeyword(1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return checkKeyword(1, 3, "lse", TOKEN_ELSE);
  case 'f':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'a':
        return checkKeyword(2, 3, "lse", TOKEN_FALSE);
      case 'o':
        return checkKeyword(2, 1, "r", TOKEN_FOR);
      case 'u':
        return checkKeyword(2, 1, "n", TOKEN_FUN);
      default:
        break;
      }
    }
    break;

  case 'i':
    return checkKeyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return checkKeyword(1, 2, "il", TOKEN_NIL);
  case 'o':
    return checkKeyword(1, 1, "r", TOKEN_OR);
  case 'p':
    return checkKeyword(1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return checkKeyword(1, 4, "uper", TOKEN_SUPER);
  case 't':
    if (scanner.current - scanner.start > 1) {
      switch (scanner.start[1]) {
      case 'h':
        return checkKeyword(2, 2, "is", TOKEN_THIS);
      case 'r':
        return checkKeyword(2, 2, "ue", TOKEN_TRUE);
      default:
        break;
      }
    }
    break;
  case 'v':
    return checkKeyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return checkKeyword(1, 4, "hile", TOKEN_WHILE);
  default:
    break;
  }
  return TOKEN_IDENTIFIER;
}
static Token identifier(void) {
  while (isAlpha(peek()) || isDigit(peek()))
    advance();
  return makeToken(identifierType());
}
static void skipWhitespace(void) { // Skip whiteskip skip everything that is not a
                               // part of the token like space,tab
  for (;;) {
    char c = peek();
    switch (c) {
    case '\n':
      scanner.line++;
      advance();
      break;
    case '/':
      if (peekNext() == '/') {
        while (peek() != '\n' || isAtEnd())
          advance();
      } else {
        return;
      }
      break;

    case '\t':
    case ' ':
    case '\r':
      advance();
      break;
    default:
      return;
    }
  }
}

Token scanToken(void) {
  skipWhitespace();
  scanner.start = scanner.current;

  if (isAtEnd())
    return makeToken(TOKEN_EOF);
  char c = advance();
  if (isAlpha(c))
    return identifier();
  if (isDigit(c))
    return number();
  switch (c) {
  case '{':
    return makeToken(TOKEN_LEFT_BRACE);
  case '}':
    return makeToken(TOKEN_RIGHT_BRACE);
  case '(':
    return makeToken(TOKEN_LEFT_PRACE);
  case ')':
    return makeToken(TOKEN_RIGHT_PRACE);
  case ';':
    return makeToken(TOKEN_SEMICOLON);
  case ',':
    return makeToken(TOKEN_COMMA);
  case '.':
    return makeToken(TOKEN_DOT);
  case '-':
    return makeToken(TOKEN_MINUS);
  case '+':
    return makeToken(TOKEN_PLUS);
  case '/':
    return makeToken(TOKEN_SLASH);
  case '*':
    return makeToken(TOKEN_STAR);

    // matching double character.
  case '!':
    return makeToken(match('=') ? TOKEN_BANG : TOKEN_BANG_EQUAL);
  case '=':
    return makeToken(match('=') ?TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '>':
    return makeToken(match('=') ? TOKEN_GREATER : TOKEN_GREATER_EQUAL);
  case '<':
    return makeToken(match('=') ? TOKEN_LESS : TOKEN_LESS_EQUAL);
  case '"':
    return string();
  default:return errorToken("Unexpected character.");
  }

}
