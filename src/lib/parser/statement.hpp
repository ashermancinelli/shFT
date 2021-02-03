#pragma once

#include <boost/spirit/home/x3.hpp>
#include "ast.hpp"

namespace fortran {
  namespace parser {
    namespace x3 = boost::spirit::x3;
    using statement_type = x3::rule<struct statement, ast::statement_list>;
    using statement_id = statement_type::id;
    BOOST_SPIRIT_DECLARE(statement_type);
  }

  parser::statement_type const& statement();
}
