#include "lexer.h"
#include "parser.h"
#include <gtest/gtest.h>

TEST(Parser, empty_function) {
  const char *file = "analyze function -> int\n"
                     "jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);
  Parser p(tokens);
  auto nodes = p.parse_tokens();

  EXPECT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes.front()->type.base, BaseType::Int);
  EXPECT_TRUE(p.is_at_end());
}

TEST(Parser, int_x_is_5) {
  const char *file = "int x = 5;";

  Lexer l;
  auto tokens = l.lex_file(file);
  Parser p(tokens);
  auto nodes = p.parse_tokens();

  EXPECT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes.front()->node_type, ASTNodeType::Declaration);

  std::string identifier(nodes.front()->identifier,
                         nodes.front()->identifier_length);
  EXPECT_STREQ(identifier.c_str(), "x");

  auto rhs = nodes.front()->rhs;
  EXPECT_NE(rhs, nullptr);
  EXPECT_EQ(rhs->long_val, 5);
  EXPECT_EQ(rhs->ulong_val, 0);
  EXPECT_EQ(rhs->float_val, 0);

  EXPECT_TRUE(p.is_at_end());
}
