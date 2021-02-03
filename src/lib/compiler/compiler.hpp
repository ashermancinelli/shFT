#pragma once

#include <ast.hpp>
#include <error_handler.hpp>
#include <vector>
#include <map>
#include <cassert>

namespace fortran::compiler {
  struct program {
    void op(int a);
    void op(int a, int b);
    void op(int a, int b, int c);

    int& operator[](std::size_t i) { return code[i]; }
    int operator[](std::size_t i) const { return code[i]; }
    void clear() { code.clear(); variables.clear(); }
    std::size_t size() const { return code.size(); }
    std::vector<int> const& operator()() const { return code; }

    std::size_t nvars() const { return variables.size(); }
    int const* find_var(std::string const& name) const;
    void add_var(std::string const& name);

    void print_variables(std::vector<int> const& stack) const;
    void print_assembler() const;

  private:

    std::map<std::string, int> variables;
    std::vector<int> code;
  };

  struct compiler {
    using result_type = bool;
    using error_handler_type = std::function<
      void(x3::position_tagged, std::string const&)>;

    template<typename ErrorHandler>
    compiler(::fortran::compiler::program& prog,
        ErrorHandler const& error_handler) : program_(prog), error_handler_(
          [&](x3::position_tagged pos, std::string const& msg) {
            error_handler(pos, msg); }) {}

    inline bool operator()(ast::null) const {
      assert(false && "Attempted to execute null command!");
      return false;
    }

    bool operator()(unsigned int x) const;
    bool operator()(bool x) const;
    bool operator()(ast::variable const& x) const;
    bool operator()(ast::operation const& x) const;
    bool operator()(ast::unary const& x) const;
    bool operator()(ast::expression const& x) const;
    bool operator()(ast::assignment const& x) const;
    bool operator()(ast::variable_declaration const& x) const;
    bool operator()(ast::statement_list const& x) const;
    bool operator()(ast::statement const& x) const;
    bool operator()(ast::if_statement const& x) const;
    bool operator()(ast::while_statement const& x) const;

    bool start(ast::statement_list const& x) const;

    client::code_gen::program& program;
    error_handler_type error_handler;
  };
}
