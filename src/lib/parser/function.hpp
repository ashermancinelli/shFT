#pragma once

#include <list>
#include "ast.hpp"
#include "statement.hpp"
#include "skipper.hpp"

namespace fortran::parser {
  /*****************************************************************************
   * Function Grammar
   *
   * The main grammar of the language is defined as a list of free functions
   * at the moment, so in this source the top-level rule will be defined.
   ****************************************************************************/

  template <typename Iterator>
  struct function : qi::grammar<Iterator, ast::function(), skipper<Iterator> >
  {
    function(error_handler<Iterator>& error_handler);

    statement<Iterator> body;
    qi::rule<Iterator, std::string(), skipper<Iterator> > name;
    qi::rule<Iterator, ast::identifier(), skipper<Iterator> > identifier;
    qi::rule<Iterator, std::list<ast::identifier>(), skipper<Iterator> > argument_list;
    qi::rule<Iterator, ast::function(), skipper<Iterator> > start;
  };
}
