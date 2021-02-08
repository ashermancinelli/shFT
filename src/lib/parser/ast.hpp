#pragma once

#include <boost/config/warning_disable.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/optional.hpp>
#include <list>

namespace shFT::ast {
  struct null {};
  struct unary;
  struct function_call;
  struct expression;

  /* Used to annotate the AST with the iterator position. This id is used as a
   * key to a map<int, Iterator> (not really part of the AST.) */
  struct tagged {
    int id;
  };

  struct identifier : tagged {
    identifier(std::string const& name = "") : name_(name) {}
    std::string name_;
  };

  using operand = boost::variant<
    null
    , unsigned int
    , identifier
    , boost::recursive_wrapper<unary>
    , boost::recursive_wrapper<expression>
    , boost::recursive_wrapper<function_call>
  >;

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

  struct operation : tagged {
    optoken operator_;
    operand operand_;
  };

  struct function_call : tagged {
    identifier function_name_;
    std::list<expression> args_;
  };

  struct expression : tagged {
    operand first_;
    std::list<operation> rest_;
  };

  struct assignment : tagged {
    identifier lhs_;
    expression rhs_;
  };

  struct variable_declaration {
    identifier lhs_;
    std::optional<expression> rhs_;
  };

  struct if_statement;
  struct while_statement;
  struct statement_list;
  struct _return;

  using statement = boost::variant<
    variable_declaration
    , assignment
    , boost::recursive_wrapper<if_statement>
    , boost::recursive_wrapper<while_statement>
    , boost::recursive_wrapper<statement_list>
    , boost::recursive_wrapper<_return>
  >;

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

  struct _return : tagged {
    std::optional<expression> expr_;
  };

  struct function : tagged {
    std::string return_type_;
    identifier function_name_;
    std::list<identifier> args_;
    statement_list body_;
  };

  using function_list = std::list<function>;

  inline std::ostream& operator<<(std::ostream& out, identifier const& var) {
    out << var.name_;
    return out;
  }
}

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::unary,
    operator_, operand_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::operation,
    operator_, operand_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::function_call,
    function_name_, args_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::expression,
    first_, rest_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::variable_declaration,
    lhs_, rhs_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::assignment,
    lhs_, rhs_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::if_statement,
    condition_, body_, else_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::while_statement,
    condition_, body_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::function,
    return_type_, function_name_, args_, body_);

BOOST_FUSION_ADAPT_STRUCT(shFT::ast::_return,
    expr_);
