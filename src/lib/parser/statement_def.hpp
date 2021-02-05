#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include "ast.hpp"
#include "ast_adapt.hpp"
#include "statement.hpp"
#include "expression.hpp"
#include "common.hpp"
#include "error_handler.hpp"

namespace fortran::parser {
  using x3::raw;
  using x3::lexeme;
  using namespace x3::ascii;

  x3::rule<struct statement_list, ast::statement_list> const
    statement_list("statement_list");

  x3::rule<struct variable_declaration, ast::variable_declaration> const
    variable_declaration("variable_declaration");

  x3::rule<struct assignment, ast::assignment> const assignment("assignment");

  x3::rule<struct variable, ast::variable> const variable("variable");

  statement_type const statement("statement");

  namespace {
    auto const& expression = fortran::expression();
  }

  auto const statement_list_def =
    +(variable_declaration | assignment)
    ;

  auto const variable_declaration_def =
    lexeme["integer" >> !(alnum | '_')] // make sure we have whole words
    >   assignment
    ;

  auto const assignment_def =
    variable
    >   '='
    >   expression
    >   ';'
    ;

  auto const variable_def = identifier;
  auto const statement_def = statement_list;

  BOOST_SPIRIT_DEFINE(
    statement
    , statement_list
    , variable_declaration
    , assignment
    , variable
  );

  struct statement_class : error_handler_base, x3::annotate_on_success {};
  struct assignment_class : x3::annotate_on_success {};
  struct variable_class : x3::annotate_on_success {};
}

namespace fortran {
  parser::statement_type const& statement() {
    return parser::statement;
  }
}
