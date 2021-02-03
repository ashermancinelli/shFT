#pragma once

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/io.hpp>
#include <optional>
#include <list>

namespace fortran::ast {
  namespace x3 = boost::spirit::x3;

  struct null {};
  struct unary;
  struct expression;

  struct variable : x3::position_tagged {
    variable(std::string const& name = "") : name_(name) {}
    std::string name_;
  };

  struct operand :
    x3::variant<
      null,
      int,
      variable,
      x3::forward_ast<unary>,
      x3::forward_ast<expression>> {
    using base_type::base_type;
    using base_type::operator=;
  };

  enum optoken {
    plus_,
    minus_,
    times_,
    divide_,
    positive_,
    negative_,
    not_,
    equal_,
    not_equal_,
    less_,
    less_equal_,
    greater_,
    greater_equal_,
    and_,
    or_,
  };

  struct unary {
    optoken operator_;
    operand operand_;
  };

  struct operation : x3::position_tagged {
    optoken operator_;
    operand operand_;
  };

  struct expression : x3::position_tagged {
    operand first_;
    std::list<operation> rest_;
  };

  struct assignment : x3::position_tagged {
    variable lhs_;
    expression rhs_;
  };

  struct variable_declaration {
    assignment assignment_;
  };

  struct if_statement;
  struct while_statement;
  struct statement_list;

  struct statement :
    x3::variant<
      variable_declaration,
      assignment,
      boost::recursive_wrapper<if_statement>,
      boost::recursive_wrapper<while_statement>,
      boost::recursive_wrapper<statement_list>
    > {
    using base_type::base_type;
    using base_type::operator=;
  };

  struct statement_list : std::list<statement> {};

  struct if_statement {
    expression condition_;
    statement body_;
    std::optional<statement> else_;
  };

  struct while_statement {
    expression condition_;
    statement body_;
  };

  inline std::ostream& operator<<(std::ostream& out, variable const& var) {
    out << var.name_;
    return out;
  }
}
