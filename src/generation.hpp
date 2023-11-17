#pragma once

#include "./parser.hpp"
#include <bits/stdc++.h>
#include <sstream>
#include <string>

class Generator {
public:
  inline Generator(NodeProgram program) : m_program(std::move(program)) {}

  void gen_expr(const NodeExpr &expr) {
    struct ExprVisitor {
      Generator *gen;

      void operator()(const NodeExprIntLit &expr_int_lit) const {
        gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value()
                      << "\n";
        gen->push("rax");
      }

      void operator()(const NodeExprIdent &expr_ident) {
        if (!gen->m_vars.contains(expr_ident.ident.value.value())) {
          std::cerr << "Undeclared Variable " << expr_ident.ident.value.value()
                    << std::endl;
          exit(EXIT_FAILURE);
        }
        const auto &var = gen->m_vars.at(expr_ident.ident.value.value());
        std::stringstream offset;
        offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1) * 8
               << "]\n";
        gen->push(offset.str());
      }
    };

    ExprVisitor visitor{.gen = this};
    std::visit(visitor, expr.var);
  }

  void generate_statement(const NodeStmt &stmt) {
    struct StmtVisitor {
      Generator *gen;
      void operator()(const NodeStmtExit &stmt_exit) const {
        gen->gen_expr(stmt_exit.expr);
        gen->m_output << "    mov rax, 60\n";
        gen->pop("rdi");
        gen->m_output << "    syscall\n";
      }

      void operator()(const NodeStmtLet &stmt_let) {
        if (gen->m_vars.contains(stmt_let.ident.value.value())) {
          std::cerr << "Redeclaring variable " << stmt_let.ident.value.value()
                    << std::endl;
          exit(EXIT_FAILURE);
        }

        gen->m_vars.insert({stmt_let.ident.value.value(),
                            Var{.stack_loc = gen->m_stack_size}});
        gen->gen_expr(stmt_let.expr);
      }
    };

    StmtVisitor visitor{.gen = this};
    std::visit(visitor, stmt.var);
  }

  [[nodiscard]] std::string generate_program() {
    m_output << "global _start\n_start:\n";

    for (const NodeStmt &stmt : m_program.stmts) {
      generate_statement(stmt);
    }
    m_output << "    mov rax, 60\n";
    m_output << "    mov rdi, 0\n";
    m_output << "    syscall\n";
    return m_output.str();
  }

private:
  void push(const std::string &reg) {
    m_output << "    push " << reg << "\n";
    m_stack_size++;
  }

  void pop(const std::string &reg) {
    m_output << "    pop " << reg << "\n";
    m_stack_size--;
  }

  struct Var {
    size_t stack_loc;
  };

  const NodeProgram m_program;
  std::stringstream m_output;
  size_t m_stack_size = 0;
  std::unordered_map<std::string, Var> m_vars;
};
