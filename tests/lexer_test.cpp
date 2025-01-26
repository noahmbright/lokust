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
  const char *file = "analyze function -> int jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);

  EXPECT_EQ(tokens.size(), 7);
  EXPECT_EQ(tokens[0].type, TokenType::Analyze);
  EXPECT_EQ(tokens[1].type, TokenType::Identifier);
  EXPECT_STREQ(std::string(tokens[1].identifier, tokens[1].length).c_str(),
               "function");
  EXPECT_EQ(tokens[2].type, TokenType::Arrow);
  EXPECT_EQ(tokens[3].type, TokenType::Int);
  EXPECT_EQ(tokens[4].type, TokenType::Jart);
  EXPECT_EQ(tokens[5].type, TokenType::Joever);
  EXPECT_EQ(tokens[6].type, TokenType::Eof);
}
