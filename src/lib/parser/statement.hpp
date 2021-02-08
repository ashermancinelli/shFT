#pragma once

#include "expression.hpp"

namespace fortran::parser {
  /*****************************************************************************
   * Statement Grammar
   ****************************************************************************/

  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  using boost::spirit::lexeme;
  using ascii::space;
  using ascii::alpha;
  using ascii::alnum;

  template <typename Iterator>
  struct statement : qi::grammar<
                     Iterator
                     , ast::statement_list()
                     , skipper<Iterator>
                     > {
    statement(error_handler<Iterator>& error_handler);

    expression<Iterator> expr;

    qi::rule<Iterator, ast::statement_list(), skipper<Iterator>> statement_list,
      compound_statement;

    qi::rule<Iterator, ast::statement(), skipper<Iterator>> statement_;

    qi::rule<Iterator, ast::variable_declaration(), skipper<Iterator>>
      variable_declaration;

    qi::rule<Iterator, ast::assignment(), skipper<Iterator>> assignment;

    qi::rule<Iterator, ast::if_statement(), skipper<Iterator>> if_statement;

    qi::rule<Iterator, ast::while_statement(), skipper<Iterator>>
      while_statement;

    qi::rule<Iterator, ast::_return(), skipper<Iterator>> _return;

    qi::rule<Iterator, std::string(), skipper<Iterator>> identifier;
  };

}
