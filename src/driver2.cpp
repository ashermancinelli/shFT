#include <ast.hpp>
#include <statement.hpp>
#include <error_handler.hpp>
#include <config.hpp>
#include <iostream>

int main() {
  std::string storage, line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      break;
    }
    storage += line + '\n';
  }

  using fortran::parser::iterator_type;
  iterator_type it = storage.begin(), end = storage.end();
  fortran::ast::statement_list ast;

  using fortran::parser::error_handler_type;
  using boost::spirit::x3::with;
  error_handler_type error_handler(it, end, std::cerr);

  auto const parser =
    with<fortran::parser::error_handler_tag>(std::ref(error_handler))
    [
      fortran::statement()
    ];

  using boost::spirit::x3::ascii::space;
  bool r = phrase_parse(it, end, parser, space, ast);

  if (r && it==end) {
    std::cout << "Success.\n";
    return 0;
  }
  else {
    std::cout << "Failure.\n";
    return 1;
  }
}
