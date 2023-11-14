#pragma once

#include "./tokenization.hpp"
#include <vector>

struct NodeExpr {
  Token int_lit;
};
struct NodeExit {
  NodeExpr expr;
};

class Parser {
public:
  inline explicit Parser(std::vector<Token> tokens)
      : m_tokens(std::move(tokens)) {}

  std::optional<NodeExpr> parse_expr() {
    if (peek().has_value() && peek().value().type == TokenType::int_lit) {
      return NodeExpr{.int_lit = consume()};
    } else {
      return {};
    }
  }

  std::optional<NodeExit> parse() {
    std::optional<NodeExit> node_exit;

    while (peek().has_value()) {
      if (peek().value().type == TokenType::exit) {
        consume();
        if (auto node_expr = parse_expr()) {
          node_exit = NodeExit{.expr = node_expr.value()};
        } else {
          std::cerr << "Invalid expression exit" << std::endl;
          exit(EXIT_FAILURE);
        }

        if (peek().has_value() && peek().value().type == TokenType::semi) {
          consume();
        } else {
          std::cerr << "Invalid expression semi" << std::endl;
          exit(EXIT_FAILURE);
        }
      } else if (peek().value().type == TokenType::_return) {
        consume();
        if (auto node_parse = parse_expr()) {
        }
      }
    }
    m_index = 0;
    return node_exit;
  }

private:
  [[nodiscard]] inline std::optional<Token> peek(int ahead = 1) const {
    if (m_index + ahead > m_tokens.size()) {
      return {};
    } else {
      return m_tokens.at(m_index);
    }
  }

  inline Token consume() { return m_tokens.at(m_index++); }

  const std::vector<Token> m_tokens;
  size_t m_index = 0;
};