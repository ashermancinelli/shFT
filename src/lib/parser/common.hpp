#pragma once

#include <boost/spirit/home/x3.hpp>

namespace fortran::parser {
  using x3::raw;
  using x3::lexeme;
  using x3::alpha;
  using x3::alnum;

  x3::rule<struct identifier, std::string> const identifier = "identifier";

  auto const identifier_def = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

  BOOST_SPIRIT_DEFINE(identifier);
}
