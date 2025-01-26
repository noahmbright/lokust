#pragma once
#include "lexer.h"
#include <vector>

enum class ASTNodeType {
  Function,
};

enum class BaseType { Empty, Void, Int, Pointer };

struct Type {
  bool is_const;
  BaseType base;
};

struct ASTNode {
  const char *identifier = nullptr;
  ASTNodeType node_type;
  Type type;
};

class Parser {
public:
  Parser(const std::vector<Token> &tokens) : m_tokens(tokens), m_i(0){};
  ~Parser(){};
  std::vector<ASTNode> parse_tokens();
  bool is_at_end();

private:
  ASTNode parse_declaration();
  ASTNode parse_function();
  Type parse_type();
  ASTNode new_ast_node(ASTNodeType type);
  const Token *next_token();
  void emit_error(const char *message);

  const std::vector<Token> m_tokens;
  size_t m_i;
  const Token *current_token;
};
