#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <set>

#include "source_function.hpp"

namespace fortran::compiler {
  void function::op(int a) {
    code_.push_back(a);
    size_ += 1;
  }

  void function::op(int a, int b) {
    code_.push_back(a);
    code_.push_back(b);
    size_ += 2;
  }

  void function::op(int a, int b, int c) {
    code_.push_back(a);
    code_.push_back(b);
    code_.push_back(c);
    size_ += 3;
  }

  int const* function::find_var(std::string const& name) const {
    std::map<std::string, int>::const_iterator i = variables_.find(name);
    if (i == variables_.end())
      return 0;
    return &i->second;
  }

  void function::add_var(std::string const& name) {
    std::size_t n = variables_.size();
    variables_[name] = n;
  }

  void function::link_to(std::string const& name, std::size_t address) {
    function_calls_[address] = name;
  }

  void function::print_assembler() const {
    std::vector<int>::const_iterator pc = code_.begin() + address_;

    std::vector<std::string> locals(variables_.size());
    using pair = std::pair<std::string, int>;
    BOOST_FOREACH(pair const& p, variables_) {
      locals[p.second] = p.first;
      std::cout << "      local       "
        << p.first << ", @" << p.second << std::endl;
    }

    std::map<std::size_t, std::string> lines;
    std::set<std::size_t> jumps;

    while (pc != (code_.begin() + address_ + size_)) {
      std::string line;
      std::size_t address = pc - code_.begin();

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
          line += boost::lexical_cast<std::string>(*pc++);
          break;

        case op_true:
          line += "      op_true";
          break;

        case op_false:
          line += "      op_false";
          break;

        case op_jump: {
            line += "      op_jump     ";
            std::size_t pos = (pc - code_.begin()) + *pc++;
            if (pos == code_.size())
              line += "end";
            else
              line += boost::lexical_cast<std::string>(pos);
            jumps.insert(pos);
          }
          break;

        case op_jump_if: {
            line += "      op_jump_if  ";
            std::size_t pos = (pc - code_.begin()) + *pc++;
            if (pos == code_.size())
              line += "end";
            else
              line += boost::lexical_cast<std::string>(pos);
            jumps.insert(pos);
          }
          break;

        case op_call: {
            line += "      op_call     ";
            int nargs = *pc++;
            std::size_t jump = *pc++;
            line += boost::lexical_cast<std::string>(nargs) + ", ";
            BOOST_ASSERT(function_calls_.find(jump) != function_calls_.end());
            line += function_calls_.find(jump)->second;
          }
          break;

        case op_stk_adj:
          line += "      op_stk_adj  ";
          line += boost::lexical_cast<std::string>(*pc++);
          break;


        case op_return:
          line += "      op_return";
          break;
      }
      lines[address] = line;
    }

    std::cout << "start:" << std::endl;
    typedef std::pair<std::size_t, std::string> line_info;
    BOOST_FOREACH(line_info const& l, lines) {
      std::size_t pos = l.first;
      if (jumps.find(pos) != jumps.end())
        std::cout << pos << ':' << std::endl;
      std::cout << l.second << std::endl;
    }

    std::cout << "end:" << std::endl << std::endl;
  }

}
