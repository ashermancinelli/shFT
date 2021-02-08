#include "program.hpp"

namespace shFT::compiler {
  void program::op(std::initializer_list<int> l) {
    for (auto const v : l) {
      op(v);
    }
  }

  void program::op(int l) {
    code.push_back(l);
  }

  int const* program::find_var(std::string const& name) const {
    auto i = variables.find(name);
    if (i == variables.end())
      return 0;
    return &i->second;
  }

  void program::add_var(std::string const& name) {
    std::size_t n = variables.size();
    variables[name] = int(n);
  }

  void program::print_variables(std::vector<int> const& stack) const {
    for (auto const& p : variables) {
      std::cout << "    " << p.first << ": " << stack[p.second] << std::endl;
    }
  }

  void program::print_assembler() const
  {
    auto pc = code.begin();

    std::vector<std::string> locals(variables.size());
    typedef std::pair<std::string, int> pair;
    for (pair const& p : variables) {
      locals[p.second] = p.first;
      std::cout << "local       "
        << p.first << ", @" << p.second << std::endl;
    }

    std::map<std::size_t, std::string> lines;
    std::set<std::size_t> jumps;

    while (pc != code.end()) {
      std::string line;
      std::size_t address = pc - code.begin();

      switch (*pc++) {
        case op_neg:
          line += "      op_neg";
          break;

        case op_not:
          line += "      op_not";
          break;

        case op_add:
          line += "      op_add";
          break;

        case op_sub:
          line += "      op_sub";
          break;

        case op_mul:
          line += "      op_mul";
          break;

        case op_div:
          line += "      op_div";
          break;

        case op_eq:
          line += "      op_eq";
          break;

        case op_neq:
          line += "      op_neq";
          break;

        case op_lt:
          line += "      op_lt";
          break;

        case op_lte:
          line += "      op_lte";
          break;

        case op_gt:
          line += "      op_gt";
          break;

        case op_gte:
          line += "      op_gte";
          break;

        case op_and:
          line += "      op_and";
          break;

        case op_or:
          line += "      op_or";
          break;

        case op_load:
          line += "      op_load     ";
          line += locals[*pc++];
          break;

        case op_store:
          line += "      op_store    ";
          line += locals[*pc++];
          break;

        case op_int:
          line += "      op_int      ";
          line += std::to_string(*pc++);
          break;

        case op_true:
          line += "      op_true";
          break;

        case op_false:
          line += "      op_false";
          break;

        case op_jump: {
            line += "      op_jump     ";
            std::size_t pos = (pc - code.begin()) + *pc++;
            if (pos == code.size())
              line += "end";
            else
              line += std::to_string(pos);
            jumps.insert(pos);
          }
          break;

        case op_jump_if: {
            line += "      op_jump_if  ";
            std::size_t pos = (pc - code.begin()) + *pc++;
            if (pos == code.size())
              line += "end";
            else
              line += std::to_string(pos);
            jumps.insert(pos);
          }
          break;

        case op_stk_adj:
          line += "      op_stk_adj  ";
          line += std::to_string(*pc++);
          break;
      }
      lines[address] = line;
    }

    std::cout << "start:" << std::endl;
    for (auto const& l : lines) {
      std::size_t pos = l.first;
      if (jumps.find(pos) != jumps.end())
        std::cout << pos << ':' << std::endl;
      std::cout << l.second << std::endl;
    }

    std::cout << "end:" << std::endl;
  }
}
