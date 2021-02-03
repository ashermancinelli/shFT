#pragma once

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

/**
 * Bindings between "ast.hpp" expression struct and x3 parser
 */
namespace fortran {
  namespace parser {
    namespace x3 = boost::spirit::x3;
    using expression_type = x3::rule<
      struct expression_class,
      ::fortran::ast::expression
    >;
    BOOST_SPIRIT_DECLARE(expression_type);
  }
  parser::expression_type const& expression();
}
