#pragma once

#include <vector>
#include <map>
#include <cassert>
#include <initializer_list>
#include <ast.hpp>
#include <error_handler.hpp>
#include <vm.hpp>

namespace fortran::compiler {
  namespace x3 = boost::spirit::x3;
  struct program {
    void op(std::initializer_list<int>);
    void op(int);

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
      assert(false && "Null command is not compilable!");
      return false;
    }

    bool operator()(int x) const;
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

    fortran::compiler::program& program_;
    error_handler_type error_handler_;
  };
}
