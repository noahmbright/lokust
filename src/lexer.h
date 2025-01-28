#pragma once

#include <cstddef>
#include <iostream>
#include <vector>

enum class TokenType {
  Eof,

  Identifier,
  Number,
  Analyze, // fn

  // types
  Unsigned,
  Int,
  Long,
  Loong,

  // type modifiers
  Cheech, // const
  Choong,
  Paul,

  // control flow
  Edge, // for
  Until,
  Finishes,
  Krink, // while
  And,
  Or,
  If,
  Joever,
  Jart,

  // oop
  Horse,    // class
  Imagines, // extends

  // operators
  Plus,
  PlusPlus,
  Minus,
  Arrow,
  MinusMinus,
  PlusEquals,
  MinusEquals,
  Asterisk,
  AsteriskEquals,
  Divided,
  DividedEquals,
  Modulo,
  ModuloEquals,

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
  const char *position;
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

void print_token_identifier(Token t);
