#pragma once
#include "ast.hpp"
#include "ast_adapt.hpp"
#include "expression.hpp"
#include "common.hpp"
#include "error_handler.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace fortran::parser {
  using x3::uint_;
  using x3::char_;
  using x3::bool_;
  using x3::raw;
  using x3::lexeme;
  using namespace x3::ascii;

  x3::symbols<ast::optoken> equality_op;
  x3::symbols<ast::optoken> relational_op;
  x3::symbols<ast::optoken> logical_op;
  x3::symbols<ast::optoken> additive_op;
  x3::symbols<ast::optoken> multiplicative_op;
  x3::symbols<ast::optoken> unary_op;
  x3::symbols<> keywords;

  /** Statically add keywords to x3 symbol lists */
  void add_keywords() {
    static bool hasRun=false;
    if(hasRun)
      return;
    hasRun = true;

    logical_op.add
      ("&&", ast::and_)
      ("||", ast::or_)
      ;

    equality_op.add
      ("==", ast::equal_)
      ("!=", ast::not_equal_)
      ;

    relational_op.add
      ("<", ast::less_)
      ("<=", ast::less_equal_)
      (">", ast::greater_)
      (">=", ast::greater_equal_)
      ;

    additive_op.add
      ("+", ast::plus_)
      ("-", ast::minus_)
      ;

    multiplicative_op.add
      ("*", ast::times_)
      ("/", ast::divide_)
      ;

    unary_op.add
      ("+", ast::positive_)
      ("-", ast::negative_)
      ("!", ast::not_)
      ;

    keywords.add
      ("var")
      ("true")
      ("false")
      ("if")
      ("else")
      ("while")
      ;
  }

  /** Defined in "expression.hpp" */
  expression_type const expression = "expression";

  /** Others are defined below */

  x3::rule<struct equality_expression, ast::expression> const
    equality_expression = "equality_expression";

  x3::rule<struct relational_expression, ast::expression> const
    relational_expression = "relational_expression";

  x3::rule<struct logical_expression, ast::expression> const logical_expression
    = "logical_expression";

  x3::rule<struct additive_expression, ast::expression> const
    additive_expression = "additive_expression";

  x3::rule<struct multiplicative_expression, ast::expression> const
    multiplicative_expression = "multiplicative_expression";

  x3::rule<struct unary_expression, ast::operand> const unary_expression
    = "unary_expression";

  x3::rule<struct primary_expression, ast::operand> const primary_expression
    = "primary_expression";

  auto const logical_expression_def =
    equality_expression
    >> *(logical_op > equality_expression)
    ;

  auto const equality_expression_def =
    relational_expression
    >> *(equality_op > relational_expression)
    ;

  auto const relational_expression_def =
    additive_expression
    >> *(relational_op > additive_expression)
    ;

  auto const additive_expression_def =
    multiplicative_expression
    >> *(additive_op > multiplicative_expression)
    ;

  auto const multiplicative_expression_def =
    unary_expression
    >> *(multiplicative_op > unary_expression)
    ;

  auto const unary_expression_def =
    primary_expression
    |   (unary_op > primary_expression)
    ;

  auto const primary_expression_def =
    uint_
    |   bool_
    |   (!keywords >> identifier)
    |   '(' > expression > ')'
    ;

  auto const expression_def = logical_expression;

  BOOST_SPIRIT_DEFINE(
    expression,
    logical_expression,
    equality_expression,
    relational_expression,
    additive_expression,
    multiplicative_expression,
    unary_expression,
    primary_expression
    );

  struct unary_expression : x3::annotate_on_success {};
  struct primary_expression : x3::annotate_on_success {};
}

namespace fortran {
  parser::expression_type const& expression() {
    parser::add_keywords();
    return parser::expression;
  }
}
