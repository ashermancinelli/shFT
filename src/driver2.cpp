// #define BOOST_SPIRIT_X3_DEBUG

#include <ast.hpp>
#include <vm.hpp>
#include <ux.hpp>
#include <compiler.hpp>
#include <parser.hpp>
#include <error_handler.hpp>
#include <config.hpp>
#include <iostream>
#include <functional>

using namespace std;
using namespace fortran::ux;

int main(int argc, char** argv) {

  if (argc == 1)
    banner();

  std::string storage, line;
  prompt();
  while (std::getline(cin, line)) {
    if (line.empty()) {
      break;
    }
    else if (line == "help") {
      help();
      prompt();
      continue;
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
  fortran::compiler::compiler compile(prog, error_handler);

  auto const parser =
    with<fortran::parser::error_handler_tag>(std::ref(error_handler)) [
      fortran::get_parser()
    ];

  hr();
  cout << "Parsing...\n";
  using boost::spirit::x3::ascii::space;
  bool r;
  try {
    r = phrase_parse(it, end, parser, space, ast);
  }
  catch (std::exception& e) {
    cout << "shFT caught exception \"" << e.what() << "\" when parsing input.\n";
    hr();
    return 1;
  }

  if (!r || it!=end) {
    cout << "Failed to fully parse.\n";
    hr();
    return 1;
  }

  hr();
  cout << "Compiling...\n";
  r = compile.start(ast);

  if (r) {
    cout << "\tSuccess compiling.\n";
  }
  else {
    cout << "\tFailed to compile.\n";
    hr();
    return 1;
  }

  hr();
  cout << "Executing...\n";
  int ret = vm.execute(prog());
  cout << "\tExited with code " << ret << "\n";
  cout << "\tAssembler output:\n";
  prog.print_assembler();
  cout << "\tVariables:\n";
  prog.print_variables(vm.get_stack());
  hr();
  return 0;
}
