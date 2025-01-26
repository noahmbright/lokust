#include "parser.h"
#include "lexer.h"
#include <cassert>
#include <iostream>

bool Parser::is_at_end() { return m_i == m_tokens.size(); }

ASTNode new_ast_node(ASTNodeType type) {
  ASTNode node;
  node.node_type = type;

  return node;
}

void Parser::emit_error(const char *message) {
  std::cout << "Error, Line " << current_token->row << ", column "
            << current_token->column << '\n';
  std::cout << message;
}

Type Parser::parse_type() {
  Type t;
  t.base = BaseType::Empty;

  auto is_type_keyword = [&]() {
    auto curr = current_token->type;
    using enum TokenType;
    return curr == Int;
  };

  auto is_type_modifier = [&]() {
    auto curr = current_token->type;
    using enum TokenType;
    return curr == Cheech || curr == Paul;
  };

  if (!is_type_keyword() || is_type_modifier()) {
    return t;
  }

  while (is_type_keyword() || is_type_modifier()) {

    if (is_type_keyword()) {
      if (t.base != BaseType::Empty) {
        emit_error("Oh! You've given this type two types!\n");
      }

      switch (current_token->type) {
      case TokenType::Int:
        t.base = BaseType::Int;
        break;
      default:
        break;
      }
    }
    // is type modifier
    else {
      switch (current_token->type) {
      case TokenType::Cheech:
        t.is_const = true;
        break;
      default:
        break;
      }
    }

    next_token();

  } // end while

  return t;
}

// function declaration:
//  analyze <identifier> (-> <return type>)? jart <body> joever
ASTNode Parser::parse_function() {
  assert(current_token->type == TokenType::Analyze);

  ASTNode function_node;
  function_node.node_type = ASTNodeType::Function;

  if (next_token()->type != TokenType::Identifier) {
    emit_error("Why don't you just analyze an identifier for this function?\n");
    while (current_token->type != TokenType::Joever &&
           current_token->type != TokenType::Eof)
      next_token();
  }

  function_node.identifier = current_token->identifier;
  // print_token_identifier(*current_token);

  if (next_token()->type == TokenType::Arrow) {
    next_token();
    print_token_identifier(*current_token);
    function_node.type = parse_type();
    if (function_node.type.base == BaseType::Empty) {
      emit_error("Oh! Why don't you just tell me what type this function "
                 "should return!\n");
    }

  } else {
    Type void_type;
    void_type.base = BaseType::Void;
    function_node.type = void_type;
  }

  if (next_token()->type != TokenType::Jart) {
    emit_error("Oh! Why don't you just begin the analysis start with the jart "
               "keyword!?\n");
    while (current_token->type != TokenType::Joever &&
           current_token->type != TokenType::Eof)
      next_token();
  }

  while (current_token->type != TokenType::Joever &&
         current_token->type != TokenType::Eof) {
    // parse body
    next_token();
  }

  if (current_token->type == TokenType::Joever)
    next_token();

  return function_node;
}

const Token *Parser::next_token() {
  current_token = m_i < m_tokens.size() ? &m_tokens[m_i++] : nullptr;
  return current_token;
}

std::vector<ASTNode> Parser::parse_tokens() {
  std::vector<ASTNode> nodes;

  while ((current_token = next_token())) {
    if (current_token->type == TokenType::Analyze) {
      nodes.push_back(parse_function());
    }
  }

  return nodes;
}
