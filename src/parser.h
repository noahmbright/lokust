#pragma once
#include "lexer.h"
#include <unordered_map>
#include <vector>

enum class ASTNodeType {
  Function,
  Declaration,

  Number,
  Addition,
  Subtraction,
  Multiplication,
  Division,
};

enum class BaseType { Empty, Void, Int, Float, Pointer };

struct Type {
  bool is_const;
  BaseType base;
};

struct ASTNode {
  const char *identifier = nullptr;
  size_t identifier_length = 0;
  ASTNodeType node_type;
  Type type;

  long long long_val = 0;
  unsigned long long ulong_val = 0;
  double float_val = 0;

  ASTNode *lhs;
  ASTNode *rhs;

  std::vector<ASTNode> function_parameters;
};

using Scope = std::unordered_map<std::string, const ASTNode *>;

class Parser {
public:
  Parser(const std::vector<Token> &tokens) : m_tokens(tokens), m_i(0) {
    m_scopes.push_back({});
  };
  ~Parser(){};
  std::vector<ASTNode *> parse_tokens();
  bool is_at_end();

private:
  ASTNode *parse_declaration();
  ASTNode *parse_expression();
  ASTNode *parse_function();
  Type parse_type();
  ASTNode new_ast_node(ASTNodeType type);
  const Token *next_token();
  void emit_error(const char *message);

  const ASTNode *search_scopes(const std::string &);
  const ASTNode *search_scopes(const Token &);
  void insert_node_in_scope(const ASTNode *node);
  void pop_scope();

  bool is_type_keyword();
  bool is_type_modifier();

  const std::vector<Token> m_tokens;
  std::vector<Scope> m_scopes;
  size_t m_i;
  const Token *current_token;
};
