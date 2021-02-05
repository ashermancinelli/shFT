#pragma once

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <sstream>
#include <iostream>
#include "expression.hpp"
#include "statement.hpp"
#include <ux.hpp>

namespace fortran::parser {
  namespace x3 = boost::spirit::x3;

  struct error_counter_tag;

  /* For keeping track of number of error in a given parse step */
  template <typename Iterator>
  struct error_counter {
    inline int operator*() {
      return nerrors_;
    }
    inline int operator++() {
      return nerrors_++;
    }
  private:
    int nerrors_=0;
  };

  struct error_formatter {
    template <typename Iterator, typename Exception, typename Context>
    x3::error_handler_result operator()(Iterator& first, Iterator const& last,
        Exception const& x, Context const& context) {

      auto& counter = x3::get<error_counter_tag>(context).get();
      counter++;

      /* Construct error message */
      std::stringstream ss;
      ss << std::string(80, '-') << "\n"
        << "[ERROR " << *counter << "]: \"" << x.what() << "\"\n"
        << "Expected: " << x.which() << " here:\n";

      int prefix=0, suffix;
      auto curr = first;
      auto iter = x.where();

      /* Print up to the character where parsing failed */
      while (curr != iter) {
        prefix++;
        ss << *curr++;
      }
      while (suffix < 10 && iter != last) {
        ss << *iter++;
        suffix++;
      }

      /* Print line leading to failed character */
      ss << "\n";
      for (int i=0; i<prefix; i++) {
        ss << "~";
      }
      ss << "^\n";
      std::cout << ss.str();
      return x3::error_handler_result::fail;
    }
  };

  template<typename Iterator>
  using error_handler = x3::error_handler<Iterator>;

  using error_handler_tag = x3::error_handler_tag;

  struct error_handler_base {

    /* error handler grabs instantiated object from context */
    template <typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(Iterator& first, Iterator const& last,
        Exception const& x, Context const& context) {

      std::stringstream ss;
      ss << std::string(80, '-') << "\n"
        << "[ERROR]: \"" << x.what() << "\"\n"
        << "Expected " << x.which() << " here:\n";

      /* Get the handler from the context */
      auto& handler = x3::get<error_handler_tag>(context).get();

      /* Propagate */
      handler(x.where(), ss.str());

      return x3::error_handler_result::accept;
    }
  };
}
