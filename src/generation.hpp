#pragma once

#include "./parser.hpp"
#include <sstream>
#include <string>

class Generator {
public:
  inline Generator(NodeExit root) : m_root(std::move(root)) {}

  [[nodiscard]] std::string generate() const {
    std::stringstream out;
    out << "global _start\n_start:\n";
    out << "    mov rax, 60\n";
    out << "    mov rdi, " << m_root.expr.int_lit.value.value() << '\n';
    out << "    syscall";

    return out.str();
  }

private:
  const NodeExit m_root;
};
