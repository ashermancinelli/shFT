#pragma once

namespace fortran::compiler {
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
}
