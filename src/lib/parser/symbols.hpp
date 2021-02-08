#pragma once
#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace fortran::parser {
  using x3::uint_;
  using x3::char_;
  using x3::bool_;
  using x3::raw;
  using x3::lexeme;

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
      ("integer")
      ("return")
      ("var")
      ("true")
      ("false")
      ("if")
      ("else")
      ("while")
      ;
  }
}
