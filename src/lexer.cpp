#include "lexer.h"
#include <cstddef>
#include <cstring>
#include <iostream>

Token Lexer::lex_next_token(TokenType type, const char *identifier,
                            size_t length) {
  Token t;

  t.row = row;
  t.column = column;
  t.type = type;
  t.identifier = identifier;
  t.length = length;
  t.position = start_of_current;

  return t;
}

std::vector<Token> Lexer::lex_file(const char *file) {
  position = file;
  std::vector<Token> tokens;

  auto skip_whitespace = [&]() {
    while (*position == ' ' || *position == '\t' || *position == '\n') {
      if (*position == '\n') {
        row = 1;
        column++;
      }
      position++;
    }
  };

  auto is_numeric = [&]() { return *position >= '0' && *position <= '9'; };

  auto is_nondigit = [&]() {
    char c = *position;
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  };

  auto advance_position = [&]() { position++, row++; };

  auto current = [&]() { return *position; };

  auto make_keyword_or_identifier =
      [&](size_t offset, const std::string &suffix, TokenType type) {
        size_t length = position - start_of_current;

        if (length - offset == suffix.size() &&
            memcmp(suffix.c_str(), (void *)(start_of_current + offset),
                   suffix.size()) == 0) {
          return lex_next_token(type);
        }

        return lex_next_token(TokenType::Identifier, start_of_current, length);
      };

  // main loop
  while (tokens.empty() || tokens.back().type != TokenType::Eof) {
    skip_whitespace();
    start_of_current = position;

    // identifier, keyword
    if (is_nondigit()) {
      advance_position();
      while (is_numeric() || is_nondigit())
        advance_position();

      switch (*start_of_current) {
        // analyze
      case 'a':
        tokens.push_back(
            make_keyword_or_identifier(1, "nalyze", TokenType::Analyze));
        break;
        // cheech
      case 'c':
        tokens.push_back(
            make_keyword_or_identifier(1, "heech", TokenType::Cheech));
        break;
        // edge
      case 'e':
        tokens.push_back(make_keyword_or_identifier(1, "dge", TokenType::Edge));
        break;
        // finishes
      case 'f':
        tokens.push_back(
            make_keyword_or_identifier(1, "inishes", TokenType::Finishes));
        break;
        // horse
      case 'h':
        tokens.push_back(
            make_keyword_or_identifier(1, "orse", TokenType::Horse));
        break;
        // imagines, int, if
      case 'i': {
        switch (start_of_current[1]) {
        case 'f':
          tokens.push_back(make_keyword_or_identifier(2, "", TokenType::If));
          break;
        case 'm':
          tokens.push_back(
              make_keyword_or_identifier(2, "agines", TokenType::Imagines));
          break;
        case 'n':
          tokens.push_back(make_keyword_or_identifier(2, "t", TokenType::Int));
          break;
        }
        break;
      }

      case 'j': {
        switch (start_of_current[1]) {
        case 'a':
          tokens.push_back(
              make_keyword_or_identifier(1, "art", TokenType::Jart));
          break;
        case 'o':
          tokens.push_back(
              make_keyword_or_identifier(1, "oever", TokenType::Joever));
          break;
        }
        break;
      }
        // krink
      case 'k':
        tokens.push_back(
            make_keyword_or_identifier(1, "rink", TokenType::Krink));
        break;
        // until
      case 'u':
        tokens.push_back(
            make_keyword_or_identifier(1, "ntil", TokenType::Until));
        break;

      default:
        tokens.push_back(lex_next_token(TokenType::Identifier, start_of_current,
                                        position - start_of_current));
        break;
      }
      continue;
    }

    if (is_numeric()) {
      while (is_numeric() || current() == '.')
        advance_position();

      tokens.push_back(lex_next_token(TokenType::Number, start_of_current,
                                      position - start_of_current));
      continue;
    }

    switch (current()) {
    case '\n':
      row = 1;
      column++;
      position++;
      break;

    case '+': {
      advance_position();
      switch (current()) {
      case '+':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::PlusPlus));
        break;
      case '=':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::PlusEquals));
        break;
      default:
        tokens.push_back(lex_next_token(TokenType::Plus));
        break;
      }
      break;
    }

    case '-': {
      advance_position();
      switch (current()) {
      case '-':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::MinusMinus));
        break;
      case '=':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::MinusEquals));
        break;
      case '>':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::Arrow));
        break;
      default:
        tokens.push_back(lex_next_token(TokenType::Minus));
        break;
      }
      break;
    }

    case '!': {
      advance_position();
      switch (current()) {
      case '=':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::BangEquals));
        break;
      default:
        tokens.push_back(lex_next_token(TokenType::Bang));
        break;
      }
      break;
    }

    case '=': {
      advance_position();
      switch (current()) {
      case '=':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::EqualsEquals));
        break;

      default:
        tokens.push_back(lex_next_token(TokenType::Equals));
        break;
      }
      break;
    }

    case '<': {
      advance_position();
      switch (current()) {
      case '=':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::LessThanEquals));
        break;

      default:
        tokens.push_back(lex_next_token(TokenType::LessThan));
        break;
      }
      break;
    }

    case '>': {
      advance_position();
      switch (current()) {
      case '=':
        advance_position();
        tokens.push_back(lex_next_token(TokenType::GreaterThanEquals));
        break;

      default:
        tokens.push_back(lex_next_token(TokenType::GreaterThan));
        break;
      }

      break;
    }

    case '{':
      advance_position();
      tokens.push_back(lex_next_token(TokenType::LBrace));
      break;
    case '}':
      advance_position();
      tokens.push_back(lex_next_token(TokenType::RBrace));
      break;

    case '(':
      advance_position();
      tokens.push_back(lex_next_token(TokenType::LParen));
      break;
    case ')':
      advance_position();
      tokens.push_back(lex_next_token(TokenType::RParen));
      break;

    case '[':
      advance_position();
      tokens.push_back(lex_next_token(TokenType::LBracket));
      break;
    case ']':
      advance_position();
      tokens.push_back(lex_next_token(TokenType::RBracket));
      break;
    case ';':
      advance_position();
      tokens.push_back(lex_next_token(TokenType::Semicolon));
      break;
    case '\0':
      tokens.push_back(lex_next_token(TokenType::Eof));
      break;

    } // end main switch
  }   // end main loop

  return tokens;
}

void print_token_identifier(Token t) {
  std::cout << "Printing a token identifier: "
            << std::string(t.identifier, t.length) << std::endl;
}
