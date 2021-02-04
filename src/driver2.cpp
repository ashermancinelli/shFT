#include <ast.hpp>
#include <vm.hpp>
#include <compiler.hpp>
#include <statement.hpp>
#include <error_handler.hpp>
#include <config.hpp>
#include <iostream>
#include <functional>

using namespace std;

auto hr = [] {
  for(int i=0; i<80; i++)
    cout << '-';
  cout << "\n";
};

auto prompt = []() {
  cout << "-> ";
};

int main() {

  hr();
  cout << "Parser, Compiler, and Virtual Machine\n";
  hr();
  cout << "An empty line parses, compiles, and runs the input.\nExample:\n"
    << "\t-> var a = 123;\n"
    << "\t-> var b = 456;\n"
    << "\t-> var c = a + b * 4;\n\n";
  hr();

  std::string storage, line;
  prompt();
  while (std::getline(cin, line)) {
    if (line.empty()) {
      break;
    }
    storage += line + '\n';
    prompt();
  }

  using fortran::parser::iterator_type;
  iterator_type it = storage.begin(), end = storage.end();

  fortran::virtual_machine vm;
  fortran::compiler::program prog;
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
  bool r;
  try {
    r = phrase_parse(it, end, parser, space, ast);
  }
  catch (std::exception& e) {
    cout << "shFT caught exception \"" << e.what() << "\"when parsing input.\n";
    hr();
    return 1;
  }

  hr();
  if (r && it==end) {
    cout << "Success.\n";
    hr();
    return 0;
  }
  else {
    std::cout << "Failure.\n";
    hr();
    return 1;
  }
}
