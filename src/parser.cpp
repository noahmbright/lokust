#include "parser.h"
#include "lexer.h"
#include <cassert>
#include <iostream>

void Parser::pop_scope() {
  m_scopes.pop_back();
  assert(m_scopes.size() >= 1);
}

const ASTNode *Parser::search_scopes(const std::string &identifier) {
  int i = m_scopes.size() - 1;
  while (i >= 0) {
    if (m_scopes[i].count(identifier)) {
      return m_scopes[i][identifier];
    }
    i--;
  }

  std::cerr << "returning null\n" << std::endl;
  return nullptr;
}

const ASTNode *Parser::search_scopes(const Token &token) {
  std::string identifier(token.identifier, token.length);
  return search_scopes(identifier);
}

void Parser::insert_node_in_scope(const ASTNode *node) {
  assert(m_scopes.size());
  m_scopes.back()[std::string(node->identifier, node->identifier_length)] =
      node;
}

static void copy_token_identifier_to_ast_node(ASTNode *node,
                                              const Token *token) {
  node->identifier = token->identifier;
  node->identifier_length = token->length;
}

bool Parser::is_type_keyword() {
  auto curr = current_token->type;
  using enum TokenType;
  return curr == Int;
}

bool Parser::is_type_modifier() {
  auto curr = current_token->type;
  using enum TokenType;
  return curr == Cheech || curr == Paul;
};

bool Parser::is_at_end() { return m_i >= m_tokens.size(); }

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

ASTNode *Parser::parse_expression() {
  ASTNode *node = new ASTNode;

  switch (current_token->type) {
  case TokenType::Number: {
    long long res = 0;
    for (size_t i = 0; i < current_token->length; i++) {

      if (current_token->position[i] == '.') {
        i++;
        double float_res = double(res);
        for (size_t j = i; j < current_token->length; j++) {
          double exp = 0.1;

          if (current_token->position[j] == '.') {
            emit_error("Oh! Your float has more than 1 decimal!\n");
            while (current_token->type != TokenType::Semicolon &&
                   current_token->type != TokenType::Eof)
              next_token();
            break;
          }

          float_res += exp * (current_token->position[j] - '0');
          exp /= 10;
        }
        node->type.base = BaseType::Float;
        node->float_val = float_res;
        return node;
      }

      res = 10 * res + current_token->position[i] - '0';
    }
    node->type.base = BaseType::Int;
    node->long_val = res;
    return node;
  }

  default:
    assert(false && "Parse expression, not implemented\n");
  }

  return node;
}

ASTNode *Parser::parse_declaration() {
  ASTNode *node = new ASTNode;
  node->node_type = ASTNodeType::Declaration;

  switch (current_token->type) {
  case TokenType::Cheech:
  case TokenType::Int: {
    Type type = parse_type();
    std::cerr << "parsed type\n" << std::endl;
    node->type = type;

    if (current_token->type != TokenType::Identifier) {
      emit_error("Oh! Why don't you just tell me the identifier in this "
                 "declaration?\n");

      while (current_token->type != TokenType::Semicolon &&
             current_token->type != TokenType::Eof)
        next_token();

    } else if (current_token && search_scopes(*current_token)) {
      emit_error("Oh! This identifier has been used before!\n");

      while (current_token->type != TokenType::Semicolon &&
             current_token->type != TokenType::Eof)
        next_token();
    }

    copy_token_identifier_to_ast_node(node, current_token);
    std::cerr << "copied token to node\n" << std::endl;
    insert_node_in_scope(node);
    std::cerr << "inserted in scope\n" << std::endl;

    if (next_token()->type == TokenType::Equals) {
      next_token();
      std::cerr << "about to parse exp\n" << std::endl;
      node->rhs = parse_expression();
    }

    if (next_token()->type == TokenType::Semicolon) {
      next_token();
    } else {
      emit_error(
          "Oh! Why don't you just say what this new variable is = to, or "
          "end this line with a ;?\n");
      while (current_token->type != TokenType::Semicolon &&
             current_token->type != TokenType::Eof)
        next_token();
    }

    break;
  }

  default:
    assert(false && "Parse declaration: not implemented\n");
  }

  return node;
}

Type Parser::parse_type() {
  Type t;
  t.base = BaseType::Empty;

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
ASTNode *Parser::parse_function() {
  assert(current_token->type == TokenType::Analyze);

  ASTNode *function_node = new ASTNode;
  function_node->node_type = ASTNodeType::Function;

  if (next_token()->type != TokenType::Identifier) {
    emit_error("Why don't you just analyze an identifier for this function?\n");
    while (current_token->type != TokenType::Joever &&
           current_token->type != TokenType::Eof)
      next_token();
  }

  copy_token_identifier_to_ast_node(function_node, current_token);
  // print_token_identifier(*current_token);

  if (next_token()->type == TokenType::Arrow) {
    next_token();
    print_token_identifier(*current_token);
    function_node->type = parse_type();
    if (function_node->type.base == BaseType::Empty) {
      emit_error("Oh! Why don't you just tell me what type this function "
                 "should return!\n");
    }

  } else {
    Type void_type;
    void_type.base = BaseType::Void;
    function_node->type = void_type;
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

std::vector<ASTNode *> Parser::parse_tokens() {
  std::vector<ASTNode *> nodes;
  next_token();

  while (current_token && current_token->type != TokenType::Eof) {
    if (current_token->type == TokenType::Analyze) {
      nodes.push_back(parse_function());
    } else {
      nodes.push_back(parse_declaration());
    }
  }

  return nodes;
}
