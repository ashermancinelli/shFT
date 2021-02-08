#pragma once

#include <vector>
#include <map>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include "ast.hpp"
#include "error_handler.hpp"
#include <vm.hpp>

namespace fortran::compiler {
  struct function {
    function(std::vector<int>& code, int nargs)
      : code_(code), address_(code.size()), size_(0), nargs_(nargs) {}

    void op(int a);
    void op(int a, int b);
    void op(int a, int b, int c);
    int& operator[](std::size_t i) { return code_[address_+i]; }
    int const& operator[](std::size_t i) const { return code_[address_+i]; }
    std::size_t size() const { return size_; }
    std::size_t get_address() const { return address_; }
    int nargs() const { return nargs_; }
    int nvars() const { return variables_.size(); }
    int const* find_var(std::string const& name) const;
    void add_var(std::string const& name);
    void link_to(std::string const& name, std::size_t address);
    void print_assembler() const;

  private:
    std::map<std::string, int> variables_;
    std::map<std::size_t, std::string> function_calls_;
    std::vector<int>& code_;
    std::size_t address_;
    std::size_t size_;
    std::size_t nargs_;
  };
}
