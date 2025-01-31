#include "lexer.h"
#include "parser.h"
#include <gtest/gtest.h>

TEST(Parser, empty_function) {
  const char *file = "analyze function ()-> int\n"
                     "jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);
  Parser p(tokens);
  auto nodes = p.parse_tokens();

  EXPECT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes.front()->type.base, BaseType::Int);
  EXPECT_TRUE(p.is_at_end());
}

TEST(Parser, one_int_param) {
  const char *file = "analyze function (int x)-> int\n"
                     "jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);
  Parser p(tokens);
  auto nodes = p.parse_tokens();

  EXPECT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes.front()->function_parameters.size(), 1);
  EXPECT_EQ(nodes.front()->function_parameters.front().type.base,
            BaseType::Int);
  auto x_node = nodes.front()->function_parameters.front();
  EXPECT_EQ(x_node.type.base, BaseType::Int);
  EXPECT_STREQ(std::string(x_node.identifier, x_node.identifier_length).c_str(),
               "x");

  EXPECT_EQ(nodes.front()->type.base, BaseType::Int);
  EXPECT_TRUE(p.is_at_end());
}

TEST(Parser, two_int_params) {
  const char *file = "analyze function (int x, int y)-> int\n"
                     "jart joever ";

  Lexer l;
  auto tokens = l.lex_file(file);
  Parser p(tokens);
  auto nodes = p.parse_tokens();

  EXPECT_EQ(nodes.size(), 1);
  EXPECT_EQ(nodes.front()->function_parameters.size(), 2);

  auto x_node = nodes.front()->function_parameters.front();
  EXPECT_EQ(x_node.type.base, BaseType::Int);
  EXPECT_STREQ(std::string(x_node.identifier, x_node.identifier_length).c_str(),
               "x");

  auto y_node = nodes.front()->function_parameters.back();
  EXPECT_EQ(y_node.type.base, BaseType::Int);
  EXPECT_STREQ(std::string(y_node.identifier, y_node.identifier_length).c_str(),
               "y");

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
