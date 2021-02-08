#include <iostream>
#include <functional>
#include <vm.hpp>
#include <ux.hpp>
#include <compiler.hpp>
#include <ast.hpp>
#include <statement.hpp>
#include <function.hpp>
#include <error_handler.hpp>
#include "test_fns.hpp"

using namespace std;
using namespace fortran;
using namespace fortran::testing::compiler;

int main() {

  cout << ux::banner() << ux::hrwrap("Testing fortran compiler and runtime.");

  int fail = 0;
  using iterator_type = std::string::const_iterator;
  using error_handler_type = fortran::error_handler<iterator_type>;
  auto tcs = ::fortran::testing::compiler::testcases();
  for (auto const& testcase_ : tcs) {

    cout << "TEST::" << testcase_.name_ << "\n";

    iterator_type it = testcase_.code_.begin(), end = testcase_.code_.end();

    fortran::virtual_machine vm;
    fortran::ast::function_list ast;

    error_handler_type error_handler(it, end, std::cout);
    fortran::parser::skipper<iterator_type> skipper;
    fortran::parser::function<iterator_type> function(error_handler);
    fortran::compiler::compiler compiler(error_handler);

    bool r = false;
    try {
      r = phrase_parse(it, end, +function, skipper, ast);
    }
    catch (std::exception& e) {
      cout << "Failed testcase: "<< testcase_ << " with exception:\n"
        << e.what() << "\n";
      fail++;
      continue;
    }

    if (!r || it != end) {
      cout << "Failed testcase: "<< testcase_ << " due to incomplete parse.\n";
      fail++;
      continue;
    }

    r = compiler(ast);
    if (!r) {
      cout << "Failed testcase: " << testcase_ << " due to failed compilation.\n";
      fail++;
      continue;
    }

    auto p = compiler.find_function("main");
    if (!p) {
      cout << hrwrap("Failed to find entrypoint function `main`.");
      fail++;
      continue;
    }

    int ret = vm.execute(compiler.get_code());
    if (ret != testcase_.expect_) {
      cout << "Failed testcase: " << testcase_ << ". Actual value="
        << ret << "\nAssembly:\n";
      compiler.print_assembler();
      fail++;
      continue;
    }
  }

  if (fail) {
    cout << ux::hrwrap("Tests failed!");
  }
  else {
    cout << ux::hrwrap("Tests passed.");
  }

  return fail;
}
