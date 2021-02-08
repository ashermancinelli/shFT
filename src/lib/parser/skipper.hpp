#pragma once

#include <boost/spirit/include/qi.hpp>

namespace shFT::parser {
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  /*****************************************************************************
   * Skipper
   *
   * Skips whitespace and c-style multiline comments.
   ****************************************************************************/
  template <typename Iterator>
  struct skipper : qi::grammar<Iterator> {
    skipper() : skipper::base_type(start) {
      qi::char_type char_;
      ascii::space_type space;

      start =
        space
        |   "/*" >> *(char_ - "*/") >> "*/"
        ;
    }
    qi::rule<Iterator> start;
  };
}
