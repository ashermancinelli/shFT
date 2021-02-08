#include <iostream>
#include <functional>
#include <vm.hpp>
#include <ux.hpp>
#include <compiler.hpp>
#include <ast.hpp>
#include <statement.hpp>
#include <function.hpp>
#include <error_handler.hpp>

using namespace std;
using namespace shFT::ux;

/*******************************************************************************
 * Driver for main program.
 *
 * Parses arguments and passes strings (from cin or a file) into compiler,
 * parser, and virtual machine. See "src/lib/ux/ux.hpp" for user experience
 * related functions.
 ******************************************************************************/
int main(int argc, char** argv) {

  if (argc == 1)
    cout << banner();
  else {
    std::vector<std::string> args(argv, argv+argc);
    for (int i=1; i<args.size(); i++) {

      // Skip ahead to interpreter
      if (args[i] == "--interpret" || args[i] == "-i") {
        break;
      }

      // We want to compile a source file
      else if (args[i] == "--compile" || args[i] == "-c") {
        throw std::runtime_error("File-based compilation not yet supported.");
      }
      // We want to specify the name of the output file
      else if (args[i] == "--output" || args[i] == "-o") {
        throw std::runtime_error("File-based compilation not yet supported.");
      }
      // We want to run precompiled virtual machine bytecode in the vm
      else if (args[i] == "--run" || args[i] == "-r") {
        throw std::runtime_error("File-based compilation not yet supported.");
      }
    }
  }

  std::string storage, line;
  cout << prompt();
  while (std::getline(cin, line)) {
    if (line.empty()) {
      break;
    }
    else if (line == "help") {
      cout << help();
      cout << prompt();
      continue;
    }
    storage += line + '\n';
    cout << prompt();
  }

  using iterator_type = std::string::const_iterator;
  iterator_type it = storage.begin(), end = storage.end();

  shFT::virtual_machine vm;
  shFT::ast::function_list ast;

  using error_handler_type = shFT::error_handler<iterator_type>;
  error_handler_type error_handler(it, end, std::cout);
  shFT::parser::skipper<iterator_type> skipper;
  shFT::parser::function<iterator_type> function(error_handler);
  shFT::compiler::compiler compiler(error_handler);

  cout << hr();
  cout << "Parsing...\n";
  bool r;
  try {
    r = phrase_parse(it, end, +function, skipper, ast);
  }
  catch (std::exception& e) {
    cout << "shFT caught exception \"" << e.what()
      << "\" when parsing input.\n" << hr();
    return 1;
  }

  if (!r || it!=end) {
    cout << "Failed to fully parse.\n" << hr();
    return 1;
  }

  cout << hr() << "Compiling...\n";
  r = compiler(ast);

  if (r) {
    cout << "\tSuccessfully compiled.\n";
  }
  else {
    cout << "\tFailed to compile.\n" << hr();
    return 1;
  }

  auto p = compiler.find_function("main");
  if (!p) {
    cout << hrwrap("Failed to find entrypoint function `main`.");
    return 1;
  }

  /* 
   * TODO: check for arguments passed to `main`.
   * TODO: pass arguments to main function via commandline. See link
   * https://github.com/boostorg/spirit/blob/develop/example/qi/compiler_tutorial/mini_c/main.cpp#L100
   */

  cout << hr() << "Executing...\n";
  int ret = vm.execute(compiler.get_code());

  cout << "\tExited with code " << ret << "\n\tAssembler output:\n";
  compiler.print_assembler();
  cout << hr();

  return 0;
}
