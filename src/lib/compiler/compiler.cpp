
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <iostream>
#include <set>
#include <iostream>
#include "compiler.hpp"

namespace fortran::compiler {
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

  bool compiler::operator()(int x) const {
    program_.op({op_int, x});
    return true;
  }

  bool compiler::operator()(bool x) const {
    program_.op({x ? op_true : op_false});
    return true;
  }

  bool compiler::operator()(ast::variable const& x) const {
    int const* p = program_.find_var(x.name_);
    if (p == 0) {
      error_handler_(x, "Undeclared variable: " + x.name_);
      return false;
    }
    program_.op({op_load, *p});
    return true;
  }

  bool compiler::operator()(ast::operation const& x) const {
    if (!boost::apply_visitor(*this, x.operand_))
      return false;
    switch (x.operator_) {
      case ast::plus_: program_.op(op_add); break;
      case ast::minus_: program_.op(op_sub); break;
      case ast::times_: program_.op(op_mul); break;
      case ast::divide_: program_.op(op_div); break;

      case ast::equal_: program_.op(op_eq); break;
      case ast::not_equal_: program_.op(op_neq); break;
      case ast::less_: program_.op(op_lt); break;
      case ast::less_equal_: program_.op(op_lte); break;
      case ast::greater_: program_.op(op_gt); break;
      case ast::greater_equal_: program_.op(op_gte); break;

      case ast::and_: program_.op(op_and); break;
      case ast::or_: program_.op(op_or); break;
      default: assert(false && "Compiler could not find opcode!"); return false;
    }
    return true;
  }

  bool compiler::operator()(ast::unary const& x) const {
    if (!boost::apply_visitor(*this, x.operand_))
      return false;
    switch (x.operator_) {
      case ast::negative_: program_.op({op_neg}); break;
      case ast::not_: program_.op({op_not}); break;
      case ast::positive_: break;
      default: BOOST_ASSERT(0); return false;
    }
    return true;
  }

  bool compiler::operator()(ast::expression const& x) const
  {
    if (!boost::apply_visitor(*this, x.first_))
      return false;
    for (ast::operation const& oper : x.rest_) {
      if (!(*this)(oper))
        return false;
    }
    return true;
  }

  bool compiler::operator()(ast::assignment const& x) const {
    if (!(*this)(x.rhs_))
      return false;
    int const* p = program_.find_var(x.lhs_.name_);
    if (p == 0) {
      error_handler_(x.lhs_, "Undeclared variable: " + x.lhs_.name_);
      return false;
    }
    program_.op({op_store, *p});
    return true;
  }

  bool compiler::operator()(ast::variable_declaration const& x) const
  {
    int const* p = program_.find_var(x.assignment_.lhs_.name_);
    if (p != 0)
    {
      error_handler_(x.assignment_.lhs_, "Duplicate variable: " + x.assignment_.lhs_.name_);
      return false;
    }
    bool r = (*this)(x.assignment_.rhs_);
    if (r) // don't add the variable if the RHS fails
    {
      program_.add_var(x.assignment_.lhs_.name_);
      program_.op({op_store, *program_.find_var(x.assignment_.lhs_.name_)});
    }
    return r;
  }

  bool compiler::operator()(ast::statement const& x) const {
    return boost::apply_visitor(*this, x);
  }

  bool compiler::operator()(ast::_return const& x) const {
    if (!(*this)(x.value_))
      return false;
    program_.op(op_return);
    return true;
  }

  bool compiler::operator()(ast::block const& x) const {
    if (!(*this)(x.statement_))
      return false;
    return true;
  }

  bool compiler::operator()(ast::statement_list const& x) const {
    for (auto const& s : x) {
      if (!(*this)(s))
        return false;
    }
    return true;
  }

  bool compiler::operator()(ast::if_statement const& x) const {
    if (!(*this)(x.condition_))
      return false;
    program_.op({op_jump_if, 0});                      // we shall fill this (0) in later
    std::size_t skip = program_.size()-1;            // mark its position
    if (!(*this)(x.body_))
      return false;
    program_[skip] = int(program_.size()-skip);       // now we know where to jump to (after the if branch)

    if (x.else_) {                                    // We got an alse
      program_[skip] += 2;                         // adjust for the "else" jump
      program_.op({op_jump, 0});                     // we shall fill this (0) in later
      std::size_t exit = program_.size()-1;        // mark its position
      if (!(*this)(*x.else_))
        return false;
      program_[exit] = int(program_.size()-exit);   // now we know where to jump to (after the else branch)
    }

    return true;
  }

  bool compiler::operator()(ast::while_statement const& x) const {
    std::size_t loop = program_.size();              // mark our position
    if (!(*this)(x.condition_))
      return false;
    program_.op({op_jump_if, 0});                      // we shall fill this (0) in later
    std::size_t exit = program_.size()-1;            // mark its position
    if (!(*this)(x.body_))
      return false;
    program_.op({op_jump, int(loop-1) - int(program_.size())});         // loop back
    program_[exit] = int(program_.size()-exit);       // now we know where to jump to (to exit the loop)
    return true;
  }

  bool compiler::start(ast::statement_list const& x) const {
    program_.clear();
    // op_stk_adj 0 for now. we'll know how many variables we'll have later
    program_.op({op_stk_adj, 0});

    if (!(*this)(x)) {
      program_.clear();
      return false;
    }
    program_[1] = int(program_.nvars());              // now store the actual number of variables
    return true;
  }
}
