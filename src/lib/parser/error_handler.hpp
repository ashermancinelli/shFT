#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ux.hpp>

using namespace shFT::ux;

namespace shFT {
  /*****************************************************************************
   * Error Handler
   *
   * Creates error messages and uses source annotation to determine where the
   * parse/compilation failed.
   ****************************************************************************/
  template <typename Iterator>
  struct error_handler {
    template <typename, typename, typename>
    struct result { using type = void; };

    error_handler(Iterator first, Iterator last, std::ostream& os)
      : first(first), last(last), os_{os} {}

    template <typename Message, typename What>
      void operator()(Message const& message, What const& what, 
          Iterator err_pos) const {
        int line;
        Iterator line_start = get_pos(err_pos, line);
        std::stringstream ss_r, ss_g;
        if (err_pos != last) {
          ss_r << message << what << " line " << line << ':' << std::endl
            << reset(get_line(line_start)) << std::endl;
          for (; line_start != err_pos; ++line_start)
            ss_g << green("~");
          ss_g << green("^") << std::endl;
        }
        else {
          ss_r << "Unexpected end of file. " << message << what
            << " line " << line << std::endl;
        }
        os_ << red(ss_r.str()) << green(ss_g.str());
      }

    Iterator get_pos(Iterator err_pos, int& line) const {
      line = 1;
      Iterator i = first;
      Iterator line_start = first;
      while (i != err_pos) {
        bool eol = false;
        if (i != err_pos && *i == '\r') {
          eol = true;
          line_start = ++i;
        }
        if (i != err_pos && *i == '\n') {
          eol = true;
          line_start = ++i;
        }
        if (eol)
          ++line;
        else
          ++i;
      }
      return line_start;
    }

    std::string get_line(Iterator err_pos) const {
      Iterator i = err_pos;
      while (i != last && (*i != '\r' && *i != '\n'))
        ++i;
      return std::string(err_pos, i);
    }

    Iterator first;
    Iterator last;
    std::vector<Iterator> iters;
    std::ostream& os_;
  };
}
