#pragma once

#include <cstddef>
#include <vector>

enum class TokenType {
  Eof,

  Identifier,
  Number,

  // types
  Int,

  // type modifiers
  Cheech, // const

  // control flow
  Edge,  // while
  Krink, // for
  And,
  Or,
  If,

  // oop
  Horse,    // class
  Imagines, // extends

  // operators
  Plus,
  PlusPlus,
  Minus,
  MinusMinus,
  PlusEquals,
  MinusEquals,
  Asterisk,
  AsteriskEquals,
  Divided,
  DividedEquals,

  Equals,
  EqualsEquals,
  BangEquals,
  Bang,

  // punctuation
  LParen,
  RParen,
  LBrace,
  RBrace,
  LBracket,
  RBracket,
  LessThan,
  GreaterThan,
  LessThanEquals,
  GreaterThanEquals,
  Semicolon,
};

struct Token {
  unsigned row, column;
  TokenType type;
  const char *identifier;
  size_t length;
};

class Lexer {
public:
  Lexer() {}
  ~Lexer() {}
  std::vector<Token> lex_file(const char *file);

private:
  unsigned row = 1, column = 1;
  const char *position = 0, *start_of_current = 0;

  Token lex_next_token(TokenType type, const char *identifier = nullptr,
                       size_t length = 0);
};
