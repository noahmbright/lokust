#include "lexer.h"
#include <gtest/gtest.h>

TEST(Lexer, int_x_is_5) {
  const char *file = "int x = 5;";
  Lexer l;
  auto tokens = l.lex_file(file);

  EXPECT_EQ(tokens.size(), 6);
  EXPECT_EQ(tokens[0].type, TokenType::Int);
  EXPECT_EQ(tokens[1].type, TokenType::Identifier);
  EXPECT_EQ(tokens[1].length, 1);
  EXPECT_EQ(tokens[2].type, TokenType::Equals);
  EXPECT_EQ(tokens[3].type, TokenType::Number);
  EXPECT_EQ(tokens[3].length, 1);
  EXPECT_EQ(tokens[4].type, TokenType::Semicolon);
  EXPECT_EQ(tokens[5].type, TokenType::Eof);
}

TEST(Lexer, empty_function) {
  const char *file = "analyze function() -> int jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);

  EXPECT_EQ(tokens.size(), 9);
  EXPECT_EQ(tokens[0].type, TokenType::Analyze);
  EXPECT_EQ(tokens[1].type, TokenType::Identifier);
  EXPECT_STREQ(std::string(tokens[1].identifier, tokens[1].length).c_str(),
               "function");
  EXPECT_EQ(tokens[2].type, TokenType::LParen);
  EXPECT_EQ(tokens[3].type, TokenType::RParen);
  EXPECT_EQ(tokens[4].type, TokenType::Arrow);
  EXPECT_EQ(tokens[5].type, TokenType::Int);
  EXPECT_EQ(tokens[6].type, TokenType::Jart);
  EXPECT_EQ(tokens[7].type, TokenType::Joever);
  EXPECT_EQ(tokens[8].type, TokenType::Eof);
}

TEST(Lexer, one_int_param) {
  const char *file = "analyze function (int x)-> int\n"
                     "jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);
  EXPECT_EQ(tokens.size(), 11);
  EXPECT_EQ(tokens[0].type, TokenType::Analyze);
  EXPECT_EQ(tokens[1].type, TokenType::Identifier);
  EXPECT_STREQ(std::string(tokens[1].identifier, tokens[1].length).c_str(),
               "function");
  EXPECT_EQ(tokens[2].type, TokenType::LParen);
  EXPECT_EQ(tokens[3].type, TokenType::Int);
  EXPECT_EQ(tokens[4].type, TokenType::Identifier);
  EXPECT_STREQ(std::string(tokens[4].identifier, tokens[4].length).c_str(),
               "x");
  EXPECT_EQ(tokens[5].type, TokenType::RParen);
  EXPECT_EQ(tokens[6].type, TokenType::Arrow);
  EXPECT_EQ(tokens[7].type, TokenType::Int);
  EXPECT_EQ(tokens[8].type, TokenType::Jart);
  EXPECT_EQ(tokens[9].type, TokenType::Joever);
  EXPECT_EQ(tokens[10].type, TokenType::Eof);
}

TEST(Lexer, two_int_params) {
  const char *file = "analyze function (int x, int y)-> int\n"
                     "jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);
  EXPECT_EQ(tokens.size(), 14);
  EXPECT_EQ(tokens[0].type, TokenType::Analyze);
  EXPECT_EQ(tokens[1].type, TokenType::Identifier);
  EXPECT_STREQ(std::string(tokens[1].identifier, tokens[1].length).c_str(),
               "function");

  EXPECT_EQ(tokens[2].type, TokenType::LParen);
  EXPECT_EQ(tokens[3].type, TokenType::Int);
  EXPECT_EQ(tokens[4].type, TokenType::Identifier);
  EXPECT_STREQ(std::string(tokens[4].identifier, tokens[4].length).c_str(),
               "x");

  EXPECT_EQ(tokens[5].type, TokenType::Comma);

  EXPECT_EQ(tokens[6].type, TokenType::Int);
  EXPECT_EQ(tokens[7].type, TokenType::Identifier);
  EXPECT_STREQ(std::string(tokens[7].identifier, tokens[7].length).c_str(),
               "y");

  EXPECT_EQ(tokens[8].type, TokenType::RParen);
  EXPECT_EQ(tokens[9].type, TokenType::Arrow);
  EXPECT_EQ(tokens[10].type, TokenType::Int);
  EXPECT_EQ(tokens[11].type, TokenType::Jart);
  EXPECT_EQ(tokens[12].type, TokenType::Joever);
  EXPECT_EQ(tokens[13].type, TokenType::Eof);
}
