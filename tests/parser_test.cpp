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
  EXPECT_EQ(nodes.front().type.base, BaseType::Int);
  EXPECT_TRUE(p.is_at_end());
}
