
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <iostream>
#include <set>
#include <iostream>
#include <boost/foreach.hpp>
#include "compiler.hpp"
#include <vm.hpp>

namespace fortran::compiler {
  bool compiler::operator()(unsigned int x)
  {
    BOOST_ASSERT(current != 0);
    current->op(op_int, x);
    return true;
  }

  bool compiler::operator()(bool x)
  {
    BOOST_ASSERT(current != 0);
    current->op(x ? op_true : op_false);
    return true;
  }

  bool compiler::operator()(ast::identifier const& x)
  {
    BOOST_ASSERT(current != 0);
    int const* p = current->find_var(x.name_);
    if (p == 0)
    {
      error_handler(x.id, "Undeclared variable: " + x.name_);
      return false;
    }
    current->op(op_load, *p);
    return true;
  }

  bool compiler::operator()(ast::operation const& x)
  {
    BOOST_ASSERT(current != 0);
    if (!boost::apply_visitor(*this, x.operand_))
      return false;
    switch (x.operator_)
    {
      case ast::plus_: current->op(op_add); break;
      case ast::minus_: current->op(op_sub); break;
      case ast::times_: current->op(op_mul); break;
      case ast::divide_: current->op(op_div); break;

      case ast::equal_: current->op(op_eq); break;
      case ast::not_equal_: current->op(op_neq); break;
      case ast::less_: current->op(op_lt); break;
      case ast::less_equal_: current->op(op_lte); break;
      case ast::greater_: current->op(op_gt); break;
      case ast::greater_equal_: current->op(op_gte); break;

      case ast::and_: current->op(op_and); break;
      case ast::or_: current->op(op_or); break;
      default: BOOST_ASSERT(0); return false;
    }
    return true;
  }

  bool compiler::operator()(ast::unary const& x)
  {
    BOOST_ASSERT(current != 0);
    if (!boost::apply_visitor(*this, x.operand_))
      return false;
    switch (x.operator_)
    {
      case ast::negative_: current->op(op_neg); break;
      case ast::not_: current->op(op_not); break;
      case ast::positive_: break;
      default: BOOST_ASSERT(0); return false;
    }
    return true;
  }

  bool compiler::operator()(ast::function_call const& x)
  {
    BOOST_ASSERT(current != 0);

    if (functions.find(x.function_name_.name_) == functions.end())
    {
      error_handler(x.function_name_.id, "Function not found: " + x.function_name_.name_);
      return false;
    }

    boost::shared_ptr<fortran::compiler::function> p = functions[x.function_name_.name_];

    if (p->nargs() != x.args_.size())
    {
      error_handler(x.function_name_.id, "Wrong number of arguments: " + x.function_name_.name_);
      return false;
    }

    BOOST_FOREACH(ast::expression const& expr, x.args_)
    {
      if (!(*this)(expr))
        return false;
    }

    current->op(
        op_call,
        p->nargs(),
        p->get_address());
    current->link_to(x.function_name_.name_, p->get_address());

    return true;
  }

  bool compiler::operator()(ast::expression const& x)
  {
    BOOST_ASSERT(current != 0);
    if (!boost::apply_visitor(*this, x.first_))
      return false;
    BOOST_FOREACH(ast::operation const& oper, x.rest_)
    {
      if (!(*this)(oper))
        return false;
    }
    return true;
  }

  bool compiler::operator()(ast::assignment const& x)
  {
    BOOST_ASSERT(current != 0);
    if (!(*this)(x.rhs_))
      return false;
    int const* p = current->find_var(x.lhs_.name_);
    if (p == 0)
    {
      error_handler(x.lhs_.id, "Undeclared variable: " + x.lhs_.name_);
      return false;
    }
    current->op(op_store, *p);
    return true;
  }

  bool compiler::operator()(ast::variable_declaration const& x)
  {
    BOOST_ASSERT(current != 0);
    int const* p = current->find_var(x.lhs_.name_);
    if (p != 0)
    {
      error_handler(x.lhs_.id, "Duplicate variable: " + x.lhs_.name_);
      return false;
    }
    if (x.rhs_) // if there's an RHS initializer
    {
      bool r = (*this)(*x.rhs_);
      if (r) // don't add the variable if the RHS fails
      {
        current->add_var(x.lhs_.name_);
        current->op(op_store, *current->find_var(x.lhs_.name_));
      }
      return r;
    }
    else
    {
      current->add_var(x.lhs_.name_);
    }
    return true;
  }

  bool compiler::operator()(ast::statement const& x)
  {
    BOOST_ASSERT(current != 0);
    return boost::apply_visitor(*this, x);
  }

  bool compiler::operator()(ast::statement_list const& x)
  {
    BOOST_ASSERT(current != 0);
    BOOST_FOREACH(ast::statement const& s, x)
    {
      if (!(*this)(s))
        return false;
    }
    return true;
  }

  bool compiler::operator()(ast::if_statement const& x)
  {
    BOOST_ASSERT(current != 0);
    if (!(*this)(x.condition_))
      return false;
    current->op(op_jump_if, 0);                 // we shall fill this (0) in later
    std::size_t skip = current->size()-1;       // mark its position
    if (!(*this)(x.body_))
      return false;
    (*current)[skip] = current->size()-skip;    // now we know where to jump to (after the if branch)

    if (x.else_)                                // We got an else
    {
      (*current)[skip] += 2;                  // adjust for the "else" jump
      current->op(op_jump, 0);                // we shall fill this (0) in later
      std::size_t exit = current->size()-1;   // mark its position
      if (!(*this)(*x.else_))
        return false;
      (*current)[exit] = current->size()-exit;// now we know where to jump to (after the else branch)
    }

    return true;
  }

  bool compiler::operator()(ast::while_statement const& x)
  {
    BOOST_ASSERT(current != 0);
    std::size_t loop = current->size();         // mark our position
    if (!(*this)(x.condition_))
      return false;
    current->op(op_jump_if, 0);                 // we shall fill this (0) in later
    std::size_t exit = current->size()-1;       // mark its position
    if (!(*this)(x.body_))
      return false;
    current->op(op_jump,
        int(loop-1) - int(current->size()));    // loop back
    (*current)[exit] = current->size()-exit;    // now we know where to jump to (to exit the loop)
    return true;
  }

  bool compiler::operator()(ast::_return const& x)
  {
    if (void_return)
    {
      if (x.expr_)
      {
        error_handler(x.id, "'void' function returning a value: ");
        return false;
      }
    }
    else
    {
      if (!x.expr_)
      {
        error_handler(x.id, current_function_name + " function must return a value: ");
        return false;
      }
    }

    if (x.expr_)
    {
      if (!(*this)(*x.expr_))
        return false;
    }
    current->op(op_return);
    return true;
  }

  bool compiler::operator()(ast::function const& x)
  {
    void_return = x.return_type_ == "void";
    if (functions.find(x.function_name_.name_) != functions.end())
    {
      error_handler(x.function_name_.id, "Duplicate function: " + x.function_name_.name_);
      return false;
    }
    boost::shared_ptr<fortran::compiler::function>& p = functions[x.function_name_.name_];
    p.reset(new fortran::compiler::function(code, x.args_.size()));
    current = p.get();
    current_function_name = x.function_name_.name_;

    // op_stk_adj 0 for now. we'll know how many variables
    // we'll have later and add them
    current->op(op_stk_adj, 0);
    BOOST_FOREACH(ast::identifier const& arg, x.args_)
    {
      current->add_var(arg.name_);
    }

    if (!(*this)(x.body_))
      return false;
    (*current)[1] = current->nvars();   // now store the actual number of variables
    // this includes the arguments
    return true;
  }

  bool compiler::operator()(ast::function_list const& x)
  {
    // Jump to the main function
    code.push_back(op_jump);
    code.push_back(0); // we will fill this in later when we finish compiling
    // and we know where the main function is

    BOOST_FOREACH(ast::function const& f, x)
    {
      if (!(*this)(f))
      {
        code.clear();
        return false;
      }
    }
    // find the main function
    boost::shared_ptr<fortran::compiler::function> p =
      find_function("main");

    if (!p) // main function not found
    {
      std::cerr << "Error: main function not defined" << std::endl;
      return false;
    }
    code[1] = p->get_address()-1; // jump to this (main function) address

    return true;
  }

  void compiler::print_assembler() const
  {
    typedef std::pair<std::string, boost::shared_ptr<fortran::compiler::function> > pair;
    BOOST_FOREACH(pair const& p, functions)
    {
      std::cout << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;" << std::endl;
      std::cout << p.second->get_address() << ": function " << p.first << std::endl;
      p.second->print_assembler();
    }
  }

  boost::shared_ptr<fortran::compiler::function>
    compiler::find_function(std::string const& name) const
    {
      function_table::const_iterator i = functions.find(name);
      if (i == functions.end())
        return boost::shared_ptr<fortran::compiler::function>();
      else
        return i->second;
    }
}
